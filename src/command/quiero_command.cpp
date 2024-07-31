#include "../../include/command/quiero_command.hpp"
#include <memory>
#include <string>

QuieroCommand::QuieroCommand(std::string name, std::unique_ptr<Truco> &game)
    : name(name), game(game) {}

CommandResponse QuieroCommand::execute() const {
  CommandResponse ret;
  try {
    game->cantar_quiero(name.c_str());
    ret = CommandResponse{.route = broadcast, .msg = name + " dijo quiero\n"};
  } catch (std::exception &e) {
    ret = CommandResponse{.route = single, .msg = e.what()};
  }
  return ret;
}

std::unique_ptr<const Command>
QuieroCommand::build(std::string _, std::string name,
                     std::unique_ptr<Truco> &game) {
  return std::make_unique<QuieroCommand>(name, game);
}
