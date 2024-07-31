#pragma once

#include "command.hpp"
#include <memory>
#include <string>

class MsgCommand : public Command {
public:
  MsgCommand(std::string, std::string);
  virtual ~MsgCommand() override = default;

  virtual CommandResponse execute() const override;

  static std::unique_ptr<const Command> build(std::string, std::string);

private:
  std::string msg;
  std::string name;
};
