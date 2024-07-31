#pragma once

#include "lobby_command.hpp"
#include <cstddef>
#include <memory>

class ObserveCommand : public LobbyCommand {
public:
  ObserveCommand(size_t table_index, const AutenticatedUser *user_ptr);
  virtual ~ObserveCommand() override = default;

  virtual CommandConfirmation
  execute(std::vector<std::unique_ptr<Table>> &tables) const override;

  static std::unique_ptr<const LobbyCommand> build(std::vector<std::string>,
                                                   const AutenticatedUser *);

private:
  size_t index;
  const AutenticatedUser *user;
};
