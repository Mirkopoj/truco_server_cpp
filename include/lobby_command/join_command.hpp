#pragma once

#include "lobby_command.hpp"
#include <cstddef>
#include <memory>

class JoinCommand : public LobbyCommand {
public:
  JoinCommand(size_t table_index, const AutenticatedUser *user_ptr);
  virtual ~JoinCommand() override = default;

  virtual std::string execute(std::vector<Table> &tables) const override;

  static std::unique_ptr<const LobbyCommand> build(std::vector<std::string>,
                                                   const AutenticatedUser *);

private:
  size_t index;
  const AutenticatedUser *user;
};