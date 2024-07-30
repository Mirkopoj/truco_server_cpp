#include "../../include/lobby_command/observe_command.hpp"
#include "../../include/lexer.hpp"
#include <cstddef>
#include <stdexcept>
#include <strings.h>

ObserveCommand::ObserveCommand(size_t table_index,
                               const AutenticatedUser *user_ptr)
    : index(table_index), user(user_ptr) {}

std::string ObserveCommand::execute(std::vector<Table> &tables) const {
  try {
    tables.at(index).observe(user);
  } catch (const std::out_of_range &e) {
    return e.what();
  }
  return "Joined Succesfully as an observer";
}

std::unique_ptr<const LobbyCommand>
ObserveCommand::build(std::vector<std::string> tokens,
                      const AutenticatedUser *user) {
  return std::make_unique<ObserveCommand>(Lexer::parse_int(tokens.at(0)), user);
}
