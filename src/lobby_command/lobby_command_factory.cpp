#include "../../include/lobby_command/lobby_command_factory.hpp"
#include "../../include/lexer.hpp"
#include "../../include/lobby_command/join_command.hpp"
#include "../../include/lobby_command/list_command.hpp"
#include "../../include/lobby_command/new_command.hpp"
#include "../../include/lobby_command/observe_command.hpp"
#include "../../include/lobby_command/help_command.hpp"
#include <cctype>
#include <functional>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

typedef struct {
  std::string header;
  std::vector<std::string> predicate;
} CommandStr;

CommandStr split(std::string raw) {
  std::vector<std::string> tokens = Lexer::split(raw);
  std::string head;
  if (!tokens.empty()) {
    head = tokens.front();
    tokens.erase(tokens.begin());
  }
  return CommandStr{
      .header = head,
      .predicate = tokens,
  };
}

std::map<std::string,
         std::function<std::unique_ptr<const LobbyCommand>(
             std::vector<std::string>, const AutenticatedUser *)>> const
    LobbyCommandFactory::map = {
        {"help", LobbyHelpCommand::build},
        {"list", ListCommand::build},
        {"new", NewCommand::build},
        {"join", JoinCommand::build},
        {"observe", ObserveCommand::build},
};

std::unique_ptr<const LobbyCommand>
LobbyCommandFactory::build(std::string raw, const AutenticatedUser *user) {
  CommandStr command = split(raw);
  try {
    return map.at(command.header)(command.predicate, user);
  } catch (...) {
    throw std::runtime_error("Invalid Command");
  }
}
