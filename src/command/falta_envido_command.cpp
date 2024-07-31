#include "../../include/command/falta_envido_command.hpp"
#include <memory>
#include <string>

FaltaEnvidoCommand::FaltaEnvidoCommand(std::string name,
                                       std::unique_ptr<Truco> &game)
    : name(name), game(game) {}

CommandResponse FaltaEnvidoCommand::execute() const {
  CommandResponse ret;
  try {
    game->cantar_falta_envido(name.c_str());
    ret = CommandResponse{.route = broadcast,
                          .msg = name + " cantó falta envido\n"};
  } catch (const std::exception &e) {
    ret = CommandResponse{.route = single, .msg = e.what()};
  }
  return ret;
}

std::unique_ptr<const Command>
FaltaEnvidoCommand::build(std::string _, std::string name,
                          std::unique_ptr<Truco> &game) {
  return std::make_unique<FaltaEnvidoCommand>(name, game);
}
