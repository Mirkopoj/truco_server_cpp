#include "../include/authenticated_user.hpp"
#include "../include/lexer.hpp"
#include <algorithm>
#include <numeric>
#include <string>
#include <vector>

AutenticatedUser::AutenticatedUser(Socket socket)
    : m_socket(std::move(socket)) {
  m_socket.send("Ingrese su nombre\n");
  std::vector<std::string> name = Lexer::split(m_socket.recv());
  std::transform(name.cbegin(), name.cend() - 1, name.begin(),
                 [](std::string s) -> std::string { return s + " "; });
  m_name = std::accumulate(name.cbegin(), name.cend(), m_name);
}

std::string AutenticatedUser::name() const { return m_name; }

std::string AutenticatedUser::recv(struct pollfd *fd) const {
  return m_socket.recv(fd);
}

void AutenticatedUser::send(std::string msg) const { m_socket.send(msg); }

pollfd AutenticatedUser::get_poll_fd() const { return m_socket.get_poll_fd(); }
