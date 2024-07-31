#pragma once

#include "command.hpp"
#include <functional>
#include <map>
#include <memory>
#include <string>

class CommandFactory {
public:
  static std::unique_ptr<const Command> build(std::string);

private:
  static const std::map<
      std::string, std::function<std::unique_ptr<const Command>(std::string)>>
      map;
};
