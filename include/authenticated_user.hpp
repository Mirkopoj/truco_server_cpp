#pragma once

#include "socket.hpp"
#include <poll.h>

class AutenticatedUser {
public:
  AutenticatedUser(Socket);

  std::string name() const;

  std::string recv(struct pollfd *);
  std::string recv();
  void send(std::string);

private:
  Socket m_socket;
  std::string m_name;
};
