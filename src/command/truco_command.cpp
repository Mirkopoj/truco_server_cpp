#include "../../include/command/truco_command.hpp"
#include <memory>
#include <string>

TrucoCommand::TrucoCommand(std::string name, std::unique_ptr<Truco> &game)
    : name(name), game(game) {}

CommandResponse TrucoCommand::execute() const {
  CommandResponse ret;
  try {
    game->cantar_truco(name.c_str());
    ret = CommandResponse{.route = broadcast, .msg = name + " cantó truco\n"};
  } catch (std::exception &e) {
    ret = CommandResponse{.route = single, .msg = e.what()};
  }
  return ret;
}

std::unique_ptr<const Command>
TrucoCommand::build(std::string _, std::string name,
                    std::unique_ptr<Truco> &game) {
  return std::make_unique<TrucoCommand>(name, game);
}
