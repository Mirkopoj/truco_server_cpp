#pragma once

#include "lobby_command.hpp"
#include <cstdint>
#include <memory>

class NewCommand : public LobbyCommand {
public:
  NewCommand(uint8_t table_size, uint8_t hasta,
             const AutenticatedUser *user_ptr);
  virtual ~NewCommand() override = default;

  virtual CommandConfirmation
  execute(std::vector<std::unique_ptr<Table>> &tables) const override;

  static std::unique_ptr<const LobbyCommand> build(std::vector<std::string>,
                                                   const AutenticatedUser *);

private:
  uint8_t size;
  uint8_t limit;
  const AutenticatedUser *user;
};
