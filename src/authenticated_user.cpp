#include "../include/authenticated_user.hpp"

AutenticatedUser::AutenticatedUser(Socket socket)
    : m_socket(std::move(socket)) {
  m_socket.send("Ingrese su nombre\n");
  m_name = m_socket.recv();
}

std::string AutenticatedUser::name() const { return m_name; }

std::string AutenticatedUser::recv(struct pollfd *fd) {
  return m_socket.recv(fd);
}

std::string AutenticatedUser::recv() { return m_socket.recv(); }

void AutenticatedUser::send(std::string msg) { m_socket.send(msg); }
