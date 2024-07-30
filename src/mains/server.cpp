#include "../../include/authenticated_user.hpp"
#include "../../include/lobby_command/lobby_command.hpp"
#include "../../include/lobby_command/lobby_command_factory.hpp"
#include "../../include/msd/channel.hpp"
#include "../../include/socket.hpp"
#include "../../include/table.hpp"
#include <atomic>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <ostream>
#include <poll.h>
#include <signal.h>
#include <stdexcept>
#include <string>
#include <sys/poll.h>
#include <sys/time.h>
#include <thread>
#include <unistd.h>
#include <vector>

std::atomic<bool> quit(false);
void got_signal(int) { quit.store(true); }
int pipe_fd[2];

typedef struct {
  std::unique_ptr<const LobbyCommand> command;
  msd::channel<std::string> *channel;
} LobbyMsg;

void handle_client(Socket, msd::channel<LobbyMsg> &);

void lobby(msd::channel<LobbyMsg> &);

int main() {
  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = got_signal;
  sigfillset(&sa.sa_mask);
  sigaction(SIGINT, &sa, NULL);

  if (pipe(pipe_fd)) {
    std::cerr << "Fail to create cancel pipe" << std::endl;
    return 1;
  }

  msd::channel<LobbyMsg> channel;
  std::thread(lobby, std::ref(channel)).detach();

  uint16_t port = 1234;
  SocketListener listener(port, 5);
  std::cout << "Listening on port: " << port << std::endl;

  while (1) {
    try {
      Socket client = listener.accept();
      std::cout << "Conection from: " << client.addr() << std::endl;
      std::thread(handle_client, std::move(client), std::ref(channel)).detach();
    } catch (const std::runtime_error &e) {
      std::cerr << e.what() << std::endl;
    }
    if (quit.load()) {
      std::cerr << "SIGINT" << std::endl;
      channel << LobbyMsg{};
      write(pipe_fd[0], "", 1);
      break;
    }
  }

  close(pipe_fd[0]);
  close(pipe_fd[1]);
  std::cout << "Exiting" << std::endl;
  return 0;
}

void handle_client(Socket socket, msd::channel<LobbyMsg> &lobby_ch) {
  AutenticatedUser authenticaded_user(std::move(socket));
  msd::channel<std::string> resp;
  struct pollfd cancel_fd;
  cancel_fd.fd = pipe_fd[1];
  cancel_fd.events = POLLIN;
  while (1) {
    try {
      LobbyMsg msg = {
          .command = LobbyCommandFactory::build(
              authenticaded_user.recv(&cancel_fd), &authenticaded_user),
          .channel = &resp,
      };
      lobby_ch << std::move(msg);
      std::string res;
      resp >> res;
      authenticaded_user.send(res);
    } catch (const std::runtime_error &e) {
      if (quit.load()) {
        std::cerr << "SIGINT CLIENT" << std::endl;
        return;
      }
      authenticaded_user.send(e.what());
    }
  }
}

void lobby(msd::channel<LobbyMsg> &channel) {
  std::vector<Table> tables;
  for (const auto ch : std::move(channel)) {
    if (quit.load()) {
      break;
    }
    *(ch.channel) << ch.command->execute(tables);
  }
}
