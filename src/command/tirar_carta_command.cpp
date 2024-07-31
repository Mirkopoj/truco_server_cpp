#include "../../include/command/tirar_carta_command.hpp"
#include <cstddef>
#include <exception>
#include <memory>
#include <string>

TirarCartaCommand::TirarCartaCommand(std::string name,
                                     std::unique_ptr<Truco> &game, size_t card)
    : name(name), game(game), carta(card) {}

CommandResponse TirarCartaCommand::execute() const {
  CommandResponse ret;
  try {
    game->tirar_carta(name.c_str(), carta);
    ret = CommandResponse{.route = broadcast,
                          .msg = name + " tiró" + std::to_string(carta) + "\n"};
  } catch (const std::exception &e) {
    ret = CommandResponse{.route = single, .msg = e.what()};
  }
  return ret;
}

std::unique_ptr<const Command>
TirarCartaCommand::build(std::string carta, std::string name,
                         std::unique_ptr<Truco> &game) {
  size_t card = std::stoi(carta);
  return std::make_unique<TirarCartaCommand>(name, game, card);
}
