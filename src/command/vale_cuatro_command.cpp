#include "../../include/command/vale_cuatro_command.hpp"
#include <memory>
#include <string>

ValeCuatroCommand::ValeCuatroCommand(std::string name,
                                     std::unique_ptr<Truco> &game)
    : name(name), game(game) {}

CommandResponse ValeCuatroCommand::execute() const {
  CommandResponse ret;
  try {
    game->cantar_vale_cuatro(name.c_str());
    ret = CommandResponse{.route = broadcast,
                          .msg = name + " cantó valecuatro\n"};
  } catch (std::exception &e) {
    ret = CommandResponse{.route = single, .msg = e.what()};
  }
  return ret;
}

std::unique_ptr<const Command>
ValeCuatroCommand::build(std::string _, std::string name,
                         std::unique_ptr<Truco> &game) {
  return std::make_unique<ValeCuatroCommand>(name, game);
}
