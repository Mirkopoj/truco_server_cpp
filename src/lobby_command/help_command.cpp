#include "../../include/lobby_command/help_command.hpp"
#include <string>
#include <strings.h>

std::string HelpCommand::execute(std::vector<Table> &tables) const {
  return "help                   prints this text\n"
         "list                   prints all tables\n"
         "new COUNT[2,4,6] UP-TO creates a new table, for the given " "player count and up to the given points\n"
         "join    TABLE-ID       joins the table with " "the given id as a player\n"
         "observe TABLE-ID       joins the " "table with the given id as a observer\n";
}

std::unique_ptr<const LobbyCommand>
HelpCommand::build(std::vector<std::string> tokens,
                   const AutenticatedUser *user) {
  return std::make_unique<HelpCommand>();
}
