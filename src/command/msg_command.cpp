#include "../../include/command/msg_command.hpp"
#include <memory>
#include <string>

MsgCommand::MsgCommand(std::string s, std::string name) : msg(s), name(name) {}

CommandResponse MsgCommand::execute() const {
  return CommandResponse{.route = broadcast, .msg = name + ": " + msg};
}

std::unique_ptr<const Command> MsgCommand::build(std::string msg,
                                                 std::string name) {
  return std::make_unique<MsgCommand>(msg, name);
}
