#pragma once

#include "lobby_command.hpp"
#include <memory>

class LobbyHelpCommand : public LobbyCommand {
public:
  LobbyHelpCommand() = default;
  virtual ~LobbyHelpCommand() override = default;

  virtual CommandConfirmation
  execute(std::vector<std::unique_ptr<Table>> &tables) const override;

  static std::unique_ptr<const LobbyCommand> build(std::vector<std::string>,
                                                   const AutenticatedUser *);
};
