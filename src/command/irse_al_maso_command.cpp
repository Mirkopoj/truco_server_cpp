#include "../../include/command/irse_al_maso_command.hpp"
#include <memory>
#include <string>

IrseAlMasoCommand::IrseAlMasoCommand(std::string name,
                                     std::unique_ptr<Truco> &game)
    : name(name), game(game) {}

CommandResponse IrseAlMasoCommand::execute() const {
  CommandResponse ret;
  try {
    game->irse_al_maso(name.c_str());
    ret =
        CommandResponse{.route = broadcast, .msg = name + " se fue al maso\n"};
  } catch (std::exception &e) {
    ret = CommandResponse{.route = single, .msg = e.what()};
  }
  return ret;
}

std::unique_ptr<const Command>
IrseAlMasoCommand::build(std::string _, std::string name,
                         std::unique_ptr<Truco> &game) {
  return std::make_unique<IrseAlMasoCommand>(name, game);
}
