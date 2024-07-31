#pragma once

#include "command.hpp"
#include <memory>

class HelpCommand : public Command {
public:
  HelpCommand() = default;
  virtual ~HelpCommand() override = default;

  virtual CommandResponse execute() const override;

  static std::unique_ptr<const Command> build(std::string, std::string);
};
