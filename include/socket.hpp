#pragma once

#include <cstdint>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>

class Socket {
public:
  Socket(Socket &&);
  Socket(const Socket &) = delete;
  Socket &operator=(Socket &&) = delete;
  Socket &operator=(const Socket &) = delete;
  ~Socket();

  std::string recv(struct pollfd *fd = nullptr) const;
  void send(std::string) const;

  std::string addr() const;

  pollfd get_poll_fd() const;

private:
  friend class SocketListener;
  Socket(sockaddr_in, socklen_t, int);

  bool no_messages() const;

  sockaddr_in m_socket_addr;
  socklen_t m_socket_len;
  int m_socket_fd;
};

class SocketListener {
public:
  SocketListener(uint16_t port, int backlog);
  SocketListener(SocketListener &&) = default;
  SocketListener(const SocketListener &) = delete;
  SocketListener &operator=(SocketListener &&) = delete;
  SocketListener &operator=(const SocketListener &) = delete;
  ~SocketListener();

  Socket accept();

private:
  sockaddr_in m_socket_addr;
  int m_socket_fd;
};
