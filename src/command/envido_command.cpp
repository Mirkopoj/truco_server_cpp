#include "../../include/command/envido_command.hpp"
#include <exception>
#include <memory>
#include <string>

EnvidoCommand::EnvidoCommand(std::string name, std::unique_ptr<Truco> &game)
    : name(name), game(game) {}

CommandResponse EnvidoCommand::execute() const {
  CommandResponse ret;
  try {
    game->cantar_envido(name.c_str());
    ret = CommandResponse{.route = broadcast, .msg = name + " cantó envido\n"};
  } catch (const std::exception &e) {
    ret = CommandResponse{.route = single, .msg = e.what()};
  }
  return ret;
}

std::unique_ptr<const Command>
EnvidoCommand::build(std::string _, std::string name,
                     std::unique_ptr<Truco> &game) {
  return std::make_unique<EnvidoCommand>(name, game);
}
