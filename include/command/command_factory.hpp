#pragma once

#include "../../include/truco_domain_engine.hpp"
#include "command.hpp"
#include <functional>
#include <map>
#include <memory>
#include <string>

class CommandFactory {
public:
  static std::unique_ptr<const Command> build(std::string, std::string,
                                              std::unique_ptr<Truco> &);

private:
  static const std::map<
      std::string,
      std::function<std::unique_ptr<const Command>(std::string, std::string)>>
      map;
  static const std::map<
      std::string, std::function<std::unique_ptr<const Command>(
                       std::string, std::string, std::unique_ptr<Truco> &)>>
      truco_map;
};
