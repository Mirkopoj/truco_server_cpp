#include "../../include/lobby_command/list_command.hpp"
#include <cstddef>
#include <memory>
#include <string>
#include <strings.h>

CommandConfirmation
ListCommand::execute(std::vector<std::unique_ptr<Table>> &tables) const {
  CommandConfirmation ret;
  ret.in_table = false;
  size_t index = 0;
  for (const std::unique_ptr<Table> &table : tables) {
    ret.msg += std::to_string(index++) + ") " + table->info() + "\n";
  }
  return ret;
}

std::unique_ptr<const LobbyCommand>
ListCommand::build(std::vector<std::string> _, const AutenticatedUser *__) {
  return std::make_unique<ListCommand>();
}
