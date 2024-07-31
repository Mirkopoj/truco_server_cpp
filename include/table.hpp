#pragma once

#include "authenticated_user.hpp"
#include "msd/channel.hpp"
#include "truco_domain_engine.hpp"
#include <cstdint>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

class Table {
public:
  Table(uint8_t table_size, uint8_t hasta);
  Table(Table &&) = delete;
  Table(const Table &) = delete;
  Table &operator=(Table &&) = delete;
  Table &operator=(const Table &) = delete;
  ~Table() = default;

  void join(const AutenticatedUser *);
  void observe(const AutenticatedUser *);
  std::string info() const;

private:
  uint8_t m_size;
  uint8_t m_limit;
  std::vector<const AutenticatedUser *> m_players;
  std::vector<const AutenticatedUser *> m_observers;
  std::unique_ptr<msd::channel<Truco>> m_channel;
  std::unique_ptr<std::mutex> m_mutex;
  std::vector<const AutenticatedUser *> m_joining_players;
};
