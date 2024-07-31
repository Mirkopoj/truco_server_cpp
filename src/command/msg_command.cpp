#include "../../include/command/msg_command.hpp"
#include <memory>
#include <string>

MsgCommand::MsgCommand(std::string s) : msg(s) {}

CommandResponse MsgCommand::execute() const {
  return CommandResponse{.route = broadcast, .msg = msg};
}

std::unique_ptr<const Command> MsgCommand::build(std::string msg) {
  return std::make_unique<MsgCommand>(msg);
}
