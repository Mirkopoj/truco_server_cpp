#pragma once

#include "lobby_command.hpp"
#include <memory>
#include <string>

class ListCommand : public LobbyCommand {
public:
  ListCommand() = default;
  virtual ~ListCommand() override = default;

  virtual std::string execute(std::vector<Table> &tables) const override;

  static std::unique_ptr<const LobbyCommand> build(std::vector<std::string>,
                                                   const AutenticatedUser *);
};
