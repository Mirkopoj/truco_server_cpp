#pragma once

#include "../table.hpp"
#include <string>
#include <vector>

class LobbyCommand {
public:
  virtual ~LobbyCommand() = default;

  virtual std::string execute(std::vector<Table> &tables) const = 0;
};
