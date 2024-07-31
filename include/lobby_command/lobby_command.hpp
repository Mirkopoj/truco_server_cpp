#pragma once

#include "../table.hpp"
#include <string>
#include <vector>

typedef struct {
  bool in_table;
  std::string msg;
} CommandConfirmation;

class LobbyCommand {
public:
  virtual ~LobbyCommand() = default;

  virtual CommandConfirmation
  execute(std::vector<std::unique_ptr<Table>> &tables) const = 0;
};
