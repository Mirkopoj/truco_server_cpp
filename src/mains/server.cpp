#include "../../include/socket.hpp"
#include <cstdint>
#include <cstring>
#include <future>
#include <iostream>
#include <ostream>
#include <signal.h>
#include <stdexcept>
#include <string>
#include <sys/time.h>
#include <vector>

std::atomic<bool> quit(false);
void got_signal(int) { quit.store(true); }

void handle_client(Socket);

int main() {
  struct sigaction sa;
  memset(&sa, 0, sizeof(sa));
  sa.sa_handler = got_signal;
  sigfillset(&sa.sa_mask);
  sigaction(SIGINT, &sa, NULL);

  uint16_t port = 1234;

  SocketListener listener(port, 5);

  std::cout << "Listening on port: " << port << std::endl;

  std::vector<std::future<void>> join_handles;

  while (1) {
    try {
      Socket client = listener.accept();
      std::cout << "Conection from: " << client.addr() << std::endl;
      join_handles.push_back(
          std::async(std::launch::async, handle_client, std::move(client)));
      std::cout << "Conected" << std::endl;
    } catch (const std::runtime_error &e) {
      std::cerr << e.what() << std::endl;
    }
    if (quit.load()) {
      std::cerr << "SIGINT" << std::endl;
      break;
    }
  }

  return 0;
}

class AutenticatedUser {
public:
  AutenticatedUser(Socket);

  std::string m_name;
private:
  Socket m_socket;
};

void handle_client(Socket socket) {
  std::cout << "Cliente\n";
  AutenticatedUser authenticaded_user(std::move(socket));
  std::cout << "Autenticado: " << authenticaded_user.m_name << "\n";
}

AutenticatedUser::AutenticatedUser(Socket socket)
    : m_socket(std::move(socket)) {
  m_socket.send("Ingrese su nombre\n");
  m_name = m_socket.recv();
}
