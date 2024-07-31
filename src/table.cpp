#include "../include/table.hpp"
#include "../include/command/command_factory.hpp"
#include "../include/truco_domain_engine.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <exception>
#include <iostream>
#include <memory>
#include <mutex>
#include <optional>
#include <stdexcept>
#include <string>
#include <sys/poll.h>
#include <thread>
#include <vector>

void table_thread(std::mutex &mutex,
                  std::vector<const AutenticatedUser *> &players,
                  std::vector<const AutenticatedUser *> &observers,
                  std::vector<const AutenticatedUser *> &joining, uint8_t size,
                  uint8_t limit);

Table::Table(uint8_t table_size, uint8_t hasta)
    : m_size(table_size), m_limit(hasta),
      m_mutex(std::make_unique<std::mutex>()) {
  std::thread(table_thread, std::ref(*m_mutex), std::ref(m_players),
              std::ref(m_observers), std::ref(m_joining_players), m_size,
              m_limit)
      .detach();
}

void Table::join(const AutenticatedUser *new_player) {
  std::lock_guard<std::mutex> guard(*m_mutex);
  if (m_players.size() + m_joining_players.size() >= m_size) {
    throw std::runtime_error("Table is full\n");
  }
  m_joining_players.push_back(new_player);
}

void Table::observe(const AutenticatedUser *new_observer) {
  std::lock_guard<std::mutex> guard(*m_mutex);
  m_observers.push_back(new_observer);
}

std::string Table::info() const {
  std::lock_guard<std::mutex> guard(*m_mutex);
  return std::to_string(m_players.size() + m_joining_players.size()) + "/" +
         std::to_string(m_size) + " - " + std::to_string(m_observers.size()) +
         " observers, juego a " + std::to_string(m_limit) + " puntos";
}

std::vector<pollfd>
get_pfds(const std::vector<const AutenticatedUser *> &players) {
  std::vector<pollfd> ret(players.size());
  std::transform(
      players.cbegin(), players.cend(), ret.begin(),
      [](const AutenticatedUser *au) -> pollfd { return au->get_poll_fd(); });
  return ret;
}

void solve_joining(std::mutex &mutex,
                   std::vector<const AutenticatedUser *> &players,
                   std::vector<const AutenticatedUser *> &joining) {
  std::lock_guard<std::mutex> guard(mutex);
  for (const AutenticatedUser *player : joining) {
    players.push_back(player);
  }
  joining.clear();
}

void broadcast_msg(std::mutex &mutex,
                   std::vector<const AutenticatedUser *> &players,
                   std::vector<const AutenticatedUser *> &observers,
                   std::string msg) {
  std::lock_guard<std::mutex> guard(mutex);
  for (auto player : players) {
    player->send(msg);
  }
  for (auto observer : observers) {
    observer->send(msg);
  }
}

std::string winner(std::optional<Equipo> win) {
  if (!win.has_value()) {
    return "";
  }
  switch (win.value()) {
  case Nosotros:
    return "Nosotros\n";
  case Ellos:
    return "Ellos\n";
  }
}

void mostrar_partida(std::mutex &mutex,
                     std::vector<const AutenticatedUser *> &players,
                     std::vector<const AutenticatedUser *> &observers,
                     std::unique_ptr<Truco> &game) {
  if (game != nullptr) {
    broadcast_msg(mutex, players, observers, game->print_state() + "\n");
    std::lock_guard<std::mutex> guard(mutex);
    for (auto player : players) {
      player->send(game->print_player(player->name().c_str()) + "\n");
    }
    if (game->terminado()) {
      std::string ganador = "GANADOR:" + winner(game->ganador());
      broadcast_msg(mutex, players, observers, ganador);
      std::cout << "Terminado " << ganador << "\n";
      std::terminate();
    }
  }
}

void table_main(std::mutex &mutex,
                std::vector<const AutenticatedUser *> &players,
                std::vector<const AutenticatedUser *> &observers,
                std::unique_ptr<Truco> &game) {
  std::vector<pollfd> pfds = get_pfds(players);
  int poll_status = poll(pfds.data(), pfds.size(), 1000);
  if (poll_status == -1) {
    throw std::runtime_error("polling error");
  }
  CommandResponse res;
  for (size_t i = 0; i < players.size(); ++i) {
    if (pfds[i].revents & POLLIN) {
      try {
        res =
            CommandFactory::build(players[i]->recv(), players[i]->name(), game)
                ->execute();
        switch (res.route) {
        case single:
          players[i]->send(res.msg);
          break;
        case broadcast:
          broadcast_msg(mutex, players, observers, res.msg);
          break;
        }
      } catch (const std::runtime_error &e) {
        players[i]->send(e.what());
      }
      mostrar_partida(mutex, players, observers, game);
    }
  }
}

void try_build_game(std::mutex &mutex,
                    std::vector<const AutenticatedUser *> &players,
                    std::vector<const AutenticatedUser *> &observers,
                    std::unique_ptr<Truco> &game, uint8_t size, uint8_t limit) {
  if (players.size() == size && game == nullptr) {
    TrucoBuilder builder{};
    builder.hasta(limit);
    std::cout << "hasta: " << std::to_string(limit) << ", " << players.size()
              << " players: ";
    for (const AutenticatedUser *player : players) {
      builder.add_player(player->name().c_str());
      std::cout << player->name() << ", ";
    }
    std::cout << "\n";
    game = std::make_unique<Truco>(builder.build());
    mostrar_partida(mutex, players, observers, game);
  }
}

void table_thread(std::mutex &mutex,
                  std::vector<const AutenticatedUser *> &players,
                  std::vector<const AutenticatedUser *> &observers,
                  std::vector<const AutenticatedUser *> &joining, uint8_t size,
                  uint8_t limit) {
  std::unique_ptr<Truco> game(nullptr);
  while (1) {
    solve_joining(mutex, players, joining);
    try_build_game(mutex, players, observers, game, size, limit);
    table_main(mutex, players, observers, game);
  }
}
