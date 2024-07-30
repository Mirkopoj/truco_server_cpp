#pragma once

#include "lobby_command.hpp"
#include <functional>
#include <map>
#include <memory>
#include <string>

class LobbyCommandFactory {
public:
  static std::unique_ptr<const LobbyCommand> build(std::string,
                                                   const AutenticatedUser *);

private:
  static const std::map<std::string,
                        std::function<std::unique_ptr<const LobbyCommand>(
                            std::vector<std::string>, const AutenticatedUser *)>>
      map;
};
