#include "../include/table.hpp"
#include "../include/truco_domain_engine.hpp"
#include <cstdint>
#include <string>

Table::Table(uint8_t table_size, uint8_t hasta)
    : size(table_size), limit(hasta) {}

void Table::join(const AutenticatedUser *new_player) {
  players.push_back(new_player);
  if (players.size() == size) {
    TrucoBuilder builder;
    builder.hasta(limit);
    for (const AutenticatedUser *player : players) {
      builder.add_player(player->name().c_str());
    }
    Truco game = builder.build();
  }
}

void Table::observe(const AutenticatedUser *new_observer) {
  observers.push_back(new_observer);
}

std::string Table::info() const {
  return std::to_string(players.size()) + "/" + std::to_string(size) + " - " +
         std::to_string(observers.size()) + " observers, juego a " +
         std::to_string(limit) + " puntos";
}
