#include "../../include/command/valid_commands_command.hpp"
#include <algorithm>
#include <memory>
#include <numeric>
#include <string>
#include <vector>

ValidCommandsCommand::ValidCommandsCommand(std::string name,
                                           std::unique_ptr<Truco> &game)
    : name(name), game(game) {}

CommandResponse ValidCommandsCommand::execute() const {
  std::vector<std::string> ret = game->valid_commands(name.c_str());
  std::transform(ret.cbegin(), ret.cend(), ret.begin(),
                 [](std::string s) -> std::string { return s + "\n"; });
  std::string msg;
  return CommandResponse{
      .route = single,
      .msg = std::accumulate(name.cbegin(), name.cend(), msg),
  };
}

std::unique_ptr<const Command>
ValidCommandsCommand::build(std::string _, std::string name,
                            std::unique_ptr<Truco> &game) {
  return std::make_unique<ValidCommandsCommand>(name, game);
}
