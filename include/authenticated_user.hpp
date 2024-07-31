#pragma once

#include "socket.hpp"
#include <poll.h>

class AutenticatedUser {
public:
  AutenticatedUser(Socket);

  std::string name() const;

  std::string recv(struct pollfd * = nullptr) const;
  void send(std::string) const;

  pollfd get_poll_fd() const;

private:
  Socket m_socket;
  std::string m_name;
};
