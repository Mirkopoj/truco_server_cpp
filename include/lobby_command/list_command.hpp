#pragma once

#include "lobby_command.hpp"
#include <memory>
#include <string>

class ListCommand : public LobbyCommand {
public:
  ListCommand() = default;
  virtual ~ListCommand() override = default;

  virtual CommandConfirmation
  execute(std::vector<std::unique_ptr<Table>> &tables) const override;

  static std::unique_ptr<const LobbyCommand> build(std::vector<std::string>,
                                                   const AutenticatedUser *);
};
