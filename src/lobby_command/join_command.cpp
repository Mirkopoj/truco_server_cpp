#include "../../include/lobby_command/join_command.hpp"
#include "../../include/lexer.hpp"
#include <cstddef>
#include <stdexcept>
#include <strings.h>

JoinCommand::JoinCommand(size_t table_index, const AutenticatedUser *user_ptr)
    : index(table_index), user(user_ptr) {}

std::string JoinCommand::execute(std::vector<Table> &tables) const {
  try {
    tables.at(index).join(user);
  } catch (const std::out_of_range &e) {
    return e.what();
  }
  return "Joined Succesfully";
}

std::unique_ptr<const LobbyCommand>
JoinCommand::build(std::vector<std::string> tokens,
                   const AutenticatedUser *user) {
  return std::make_unique<JoinCommand>(Lexer::parse_int(tokens.at(0)), user);
}
