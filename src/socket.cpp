#include "../include/socket.hpp"
#include <arpa/inet.h>
#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <poll.h>
#include <stdexcept>
#include <string>
#include <strings.h>
#include <sys/poll.h>
#include <sys/socket.h>
#include <unistd.h>

SocketListener::SocketListener(uint16_t port, int backlog) {
  bzero((char *)&m_socket_addr, sizeof(m_socket_addr));
  m_socket_addr.sin_family = AF_INET;
  m_socket_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  m_socket_addr.sin_port = htons(port);

  m_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (m_socket_fd < 0) {
    throw std::runtime_error("Error establishing the server socket");
  }
  int bindStatus = bind(m_socket_fd, (struct sockaddr *)&m_socket_addr,
                        sizeof(m_socket_addr));
  if (bindStatus < 0) {
    throw std::runtime_error("Error binding socket to local address");
  }
  listen(m_socket_fd, backlog);
}

Socket SocketListener::accept() {
  sockaddr_in new_socket_addr;
  socklen_t new_socket_len = sizeof(new_socket_addr);
  int new_socket_fd = ::accept4(m_socket_fd, (sockaddr *)&new_socket_addr,
                                &new_socket_len, SOCK_NONBLOCK);
  if (new_socket_fd < 0) {
    throw std::runtime_error("Error accepting request from client!");
  }
  return Socket(new_socket_addr, new_socket_len, new_socket_fd);
}

SocketListener::~SocketListener() { close(m_socket_fd); }

Socket::Socket(sockaddr_in new_socket_addr, socklen_t new_socket_len,
               int new_socket_fd)
    : m_socket_addr(new_socket_addr), m_socket_len(new_socket_len),
      m_socket_fd(new_socket_fd) {}

Socket::Socket(Socket &&other)
    : m_socket_addr(other.m_socket_addr), m_socket_len(other.m_socket_len),
      m_socket_fd(other.m_socket_fd) {
  other.m_socket_fd = -1;
}

std::string Socket::recv(struct pollfd *fd) {
  const size_t buf_size = 1024;
  char buf[buf_size];
  std::string ret;
  int read;
  struct pollfd fds[2];
  fds[0].fd = m_socket_fd;
  fds[0].events = POLLIN;
  nfds_t fdsc = 1;
  if (fd) {
    fds[1] = *fd;
    fdsc = 2;
  }
  int poll_result = poll(fds, fdsc, -1);
  if (poll_result == -1) {
    throw std::runtime_error("Error in poll");
  }
  if (fd && fds[1].revents & POLLIN) {
    throw std::runtime_error("Cancelation");
  }
  do {
    read = ::recv(m_socket_fd, buf, buf_size, 0);
    if (read < 0) {
      if (no_messages()) {
        break;
      }
      throw std::runtime_error("Error receving message from client");
    }
    ret.append(buf);
  } while (read == buf_size);
  return ret;
}

bool Socket::no_messages() {
  int err = errno;
  return err == EWOULDBLOCK || err == EAGAIN;
}

void Socket::send(std::string msg) {
  int sent = ::send(m_socket_fd, msg.data(), msg.length(), 0);
  if (sent < 0) {
    throw std::runtime_error("Error sending message to client");
  }
}

std::string Socket::addr() {
  std::string ret("123.123.123.123");
  inet_ntop(AF_INET, &m_socket_addr, ret.data(), ret.length());
  return ret;
}

Socket::~Socket() {
  if (m_socket_fd != -1) {
    std::cout << "RIP Socket\n";
    close(m_socket_fd);
  }
}
