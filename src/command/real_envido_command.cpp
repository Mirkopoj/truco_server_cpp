#include "../../include/command/real_envido_command.hpp"
#include <memory>
#include <string>

RealEnvidoCommand::RealEnvidoCommand(std::string name,
                                     std::unique_ptr<Truco> &game)
    : name(name), game(game) {}

CommandResponse RealEnvidoCommand::execute() const {
  CommandResponse ret;
  try {
    game->cantar_real_envido(name.c_str());
    ret = CommandResponse{.route = broadcast,
                          .msg = name + " cantó realenvido\n"};
  } catch (const std::exception &e) {
    ret = CommandResponse{.route = single, .msg = e.what()};
  }
  return ret;
}

std::unique_ptr<const Command>
RealEnvidoCommand::build(std::string _, std::string name,
                         std::unique_ptr<Truco> &game) {
  return std::make_unique<RealEnvidoCommand>(name, game);
}
