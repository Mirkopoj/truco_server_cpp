#pragma once

#include "authenticated_user.hpp"
#include <cstdint>
#include <string>
#include <vector>

class Table {
public:
  Table(uint8_t table_size, uint8_t hasta);
  Table(Table &&) = default;
  Table(const Table &) = delete;
  Table &operator=(Table &&) = delete;
  Table &operator=(const Table &) = delete;
  ~Table() = default;

  void join(const AutenticatedUser *);
  void observe(const AutenticatedUser *);
  std::string info() const;

private:
  uint8_t size;
  uint8_t limit;
  std::vector<const AutenticatedUser *> players;
  std::vector<const AutenticatedUser *> observers;
};
