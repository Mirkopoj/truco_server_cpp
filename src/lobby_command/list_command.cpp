#include "../../include/lobby_command/list_command.hpp"
#include <cstddef>
#include <memory>
#include <string>
#include <strings.h>

std::string ListCommand::execute(std::vector<Table> &tables) const {
  std::string ret;
  size_t index = 0;
  for (const Table &table : tables) {
    ret += std::to_string(index++) + table.info() + "\n";
  }
  return ret;
}

std::unique_ptr<const LobbyCommand>
ListCommand::build(std::vector<std::string> _, const AutenticatedUser *__) {
  return std::make_unique<ListCommand>();
}
