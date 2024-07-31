#include "../../include/command/command_factory.hpp"
#include "../../include/command/help_command.hpp"
#include "../../include/command/msg_command.hpp"
#include "../../include/lexer.hpp"
#include <cstdio>
#include <string>
#include <sys/socket.h>

std::map<std::string,
         std::function<std::unique_ptr<const Command>(std::string)>> const
    CommandFactory::map = {
        {"help", HelpCommand::build},
};

std::unique_ptr<const Command> CommandFactory::build(std::string str) {
  std::vector<std::string> words = Lexer::split(str);
  std::string head = words.empty() ? "" : words.front();
  return map.count(head) ? map.at(head)("") : MsgCommand::build(str);
}
