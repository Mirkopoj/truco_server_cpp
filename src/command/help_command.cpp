#include "../../include/command/help_command.hpp"
#include <string>
#include <strings.h>

CommandResponse HelpCommand::execute() const {
  return CommandResponse{
      .route = single,
      .msg = "help                   prints this text\n"
             "Any other string will pass through as a message\n"};
}

std::unique_ptr<const Command> HelpCommand::build(std::string _,
                                                  std::string __) {
  return std::make_unique<HelpCommand>();
}
