#include "../../include/command/re_truco_command.hpp"
#include <memory>
#include <string>

ReTrucoCommand::ReTrucoCommand(std::string name, std::unique_ptr<Truco> &game)
    : name(name), game(game) {}

CommandResponse ReTrucoCommand::execute() const {
  CommandResponse ret;
  try {
    game->cantar_re_truco(name.c_str());
    ret = CommandResponse{.route = broadcast, .msg = name + " cantó retruco\n"};
  } catch (const std::exception &e) {
    ret = CommandResponse{.route = single, .msg = e.what()};
  }
  return ret;
}

std::unique_ptr<const Command>
ReTrucoCommand::build(std::string _, std::string name,
                      std::unique_ptr<Truco> &game) {
  return std::make_unique<ReTrucoCommand>(name, game);
}
