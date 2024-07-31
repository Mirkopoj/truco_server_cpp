#include "../../include/lobby_command/new_command.hpp"
#include "../../include/lexer.hpp"
#include <algorithm>
#include <cstdint>
#include <string>
#include <strings.h>
#include <vector>

NewCommand::NewCommand(uint8_t table_size, uint8_t hasta,
                       const AutenticatedUser *user_ptr)
    : size(table_size), limit(hasta), user(user_ptr) {}

CommandConfirmation
NewCommand::execute(std::vector<std::unique_ptr<Table>> &tables) const {
  tables.push_back(std::make_unique<Table>(size, limit));
  tables.back()->join(user);
  return CommandConfirmation{
      .in_table = true,
      .msg = "Joined Succesfully on newly created Table, up to '" +
             std::to_string(limit) + "' points and for '" +
             std::to_string(size) + "' players\n"};
}

std::unique_ptr<const LobbyCommand>
NewCommand::build(std::vector<std::string> tokens,
                  const AutenticatedUser *user) {
  std::vector<uint8_t> nums(2);
  std::transform(tokens.cbegin(), tokens.cbegin() + 2, nums.begin(),
                 Lexer::parse_int);
  return std::make_unique<NewCommand>(nums.at(0), nums.at(1), user);
}
