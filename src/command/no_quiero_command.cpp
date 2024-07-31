#include "../../include/command/no_quiero_command.hpp"
#include <memory>
#include <string>

NoQuieroCommand::NoQuieroCommand(std::string name, std::unique_ptr<Truco> &game)
    : name(name), game(game) {}

CommandResponse NoQuieroCommand::execute() const {
  CommandResponse ret;
  try {
    game->cantar_no_quiero(name.c_str());
    ret =
        CommandResponse{.route = broadcast, .msg = name + " dijo no quiero\n"};
  } catch (std::exception &e) {
    ret = CommandResponse{.route = single, .msg = e.what()};
  }
  return ret;
}

std::unique_ptr<const Command>
NoQuieroCommand::build(std::string _, std::string name,
                       std::unique_ptr<Truco> &game) {
  return std::make_unique<NoQuieroCommand>(name, game);
}
