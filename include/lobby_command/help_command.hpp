#pragma once

#include "lobby_command.hpp"
#include <memory>

class HelpCommand : public LobbyCommand {
public:
  HelpCommand() = default;
  virtual ~HelpCommand() override = default;

  virtual std::string execute(std::vector<Table> &tables) const override;

  static std::unique_ptr<const LobbyCommand> build(std::vector<std::string>,
                                                   const AutenticatedUser *);
};
