#include "../../include/lobby_command/lobby_command_factory.hpp"
#include "../../include/lexer.hpp"
#include "../../include/lobby_command/join_command.hpp"
#include "../../include/lobby_command/list_command.hpp"
#include "../../include/lobby_command/new_command.hpp"
#include "../../include/lobby_command/observe_command.hpp"
#include <cctype>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

typedef struct {
  std::string header;
  std::vector<std::string> predicate;
} CommandStr;

CommandStr split(std::string raw) {
  std::vector<std::string> tokens = Lexer::split(raw);
  for (std::string s : tokens) {
    std::cout << "-" << s << "\n";
  }
  std::string head = tokens.front();
  std::cout << "H" << head << "\n";
  if (!tokens.empty()) {
    tokens.erase(tokens.begin());
  }
  for (std::string s : tokens) {
    std::cout << "-" << s << "\n";
  }
  std::cout << "ret\n";
  return CommandStr{
      .header = head,
      .predicate = tokens,
  };
}

std::map<std::string,
         std::function<std::unique_ptr<const LobbyCommand>(
             std::vector<std::string>, const AutenticatedUser *)>> const
    LobbyCommandFactory::map = {
        {"list", ListCommand::build},
        {"new", NewCommand::build},
        {"join", JoinCommand::build},
        {"observe", ObserveCommand::build},
};

std::unique_ptr<const LobbyCommand>
LobbyCommandFactory::build(std::string raw, const AutenticatedUser *user) {
  std::cout << raw << "\n";
  CommandStr command = split(raw);
  std::cout << "H: >" << command.header << "< P: >";
  if (!command.predicate.empty()) {
    for (size_t i = 0; i < command.predicate.size() - 1; ++i) {
      std::cout << command.predicate[i] << " ";
    }
    std::cout << command.predicate[command.predicate.size() - 1] << "<\n";
  }
  return map.at(command.header)(command.predicate, user);
}
