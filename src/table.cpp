#include "../include/table.hpp"
#include "../include/command/command_factory.hpp"
#include "../include/truco_domain_engine.hpp"
#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <sys/poll.h>
#include <thread>
#include <vector>

void table_thread(msd::channel<Truco> &ch, std::mutex &mutex,
                  std::vector<const AutenticatedUser *> &players,
                  std::vector<const AutenticatedUser *> &observers,
                  std::vector<const AutenticatedUser *> &joining);

Table::Table(uint8_t table_size, uint8_t hasta)
    : m_size(table_size), m_limit(hasta),
      m_mutex(std::make_unique<std::mutex>()) {
  std::thread(table_thread, std::ref(*m_channel), std::ref(*m_mutex),
              std::ref(m_players), std::ref(m_observers),
              std::ref(m_joining_players))
      .detach();
}

void Table::join(const AutenticatedUser *new_player) {
  std::lock_guard<std::mutex> guard(*m_mutex);
  if (m_players.size() + m_joining_players.size() >= m_size) {
    throw std::runtime_error("Table is full\n");
  }
  m_joining_players.push_back(new_player);
  /*if (m_players.size() == m_size) {
    TrucoBuilder builder;
    builder.hasta(m_limit);
    std::cout << "hasta: " << std::to_string(m_limit) << ", "
              << m_players.size() << " players: ";
    for (const AutenticatedUser *player : m_players) {
      builder.add_player(player->name().c_str());
      std::cout << player->name() << ", ";
    }
    std::cout << "\n";
    Truco game = builder.build();
  }*/
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

void table_lobby(std::mutex &mutex,
                 std::vector<const AutenticatedUser *> &players,
                 std::vector<const AutenticatedUser *> &observers) {
  std::vector<pollfd> pfds = get_pfds(players);
  int poll_status = poll(pfds.data(), pfds.size(), 1000);
  if (poll_status == -1) {
    throw std::runtime_error("polling error");
  }
  CommandResponse res;
  for (size_t i = 0; i < players.size(); ++i) {
    if (pfds[i].revents & POLLIN) {
      try {
        res = CommandFactory::build(players[i]->recv())->execute();
        switch (res.route) {
        case single:
          players[i]->send(res.msg);
          break;
        case broadcast:
          std::lock_guard<std::mutex> guard(mutex);
          for (auto player : players) {
            player->send(players[i]->name() + ": " + res.msg);
          }
          for (auto observer : observers) {
            observer->send(players[i]->name() + ": " + res.msg);
          }
          break;
        }
      } catch (const std::runtime_error &e) {
        players[i]->send(e.what());
      }
    }
  }
}

void table_thread(msd::channel<Truco> &ch, std::mutex &mutex,
                  std::vector<const AutenticatedUser *> &players,
                  std::vector<const AutenticatedUser *> &observers,
                  std::vector<const AutenticatedUser *> &joining) {
  while (1) {
    solve_joining(mutex, players, joining);
    table_lobby(mutex, players, observers);
  }
}
