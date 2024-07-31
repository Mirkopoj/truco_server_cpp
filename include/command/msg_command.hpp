#pragma once

#include "command.hpp"
#include <memory>
#include <string>

class MsgCommand : public Command {
public:
  MsgCommand(std::string);
  virtual ~MsgCommand() override = default;

  virtual CommandResponse execute() const override;

  static std::unique_ptr<const Command> build(std::string);

private:
  std::string msg;
};
