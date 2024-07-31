#include "../../include/lobby_command/join_command.hpp"
#include "../../include/lexer.hpp"
#include <cstddef>
#include <exception>
#include <strings.h>

JoinCommand::JoinCommand(size_t table_index, const AutenticatedUser *user_ptr)
    : index(table_index), user(user_ptr) {}

CommandConfirmation
JoinCommand::execute(std::vector<std::unique_ptr<Table>> &tables) const {
  CommandConfirmation ret;
  try {
    tables.at(index)->join(user);
  } catch (const std::exception &e) {
    ret.in_table = false;
    ret.msg = e.what();
  }
  ret.in_table = true;
  ret.msg = "Joined Succesfully\n";
  return ret;
}

std::unique_ptr<const LobbyCommand>
JoinCommand::build(std::vector<std::string> tokens,
                   const AutenticatedUser *user) {
  return std::make_unique<JoinCommand>(Lexer::parse_int(tokens.at(0)), user);
}
