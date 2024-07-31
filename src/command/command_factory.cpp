#include "../../include/command/command_factory.hpp"
#include "../../include/command/envido_command.hpp"
#include "../../include/command/falta_envido_command.hpp"
#include "../../include/command/help_command.hpp"
#include "../../include/command/irse_al_maso_command.hpp"
#include "../../include/command/msg_command.hpp"
#include "../../include/command/no_quiero_command.hpp"
#include "../../include/command/quiero_command.hpp"
#include "../../include/command/re_truco_command.hpp"
#include "../../include/command/real_envido_command.hpp"
#include "../../include/command/tirar_carta_command.hpp"
#include "../../include/command/truco_command.hpp"
#include "../../include/command/vale_cuatro_command.hpp"
#include "../../include/command/valid_commands_command.hpp"
#include "../../include/lexer.hpp"
#include <cstdio>
#include <string>
#include <sys/socket.h>

std::map<std::string, std::function<std::unique_ptr<const Command>(
                          std::string, std::string)>> const
    CommandFactory::map = {
        {"help", HelpCommand::build},
};

std::map<std::string,
         std::function<std::unique_ptr<const Command>(
             std::string, std::string, std::unique_ptr<Truco> &)>> const
    CommandFactory::truco_map = {
        {"irse_al_maso", IrseAlMasoCommand::build},
        {"cantar_quiero", QuieroCommand::build},
        {"cantar_no_quiero", NoQuieroCommand::build},
        {"cantar_envido", EnvidoCommand::build},
        {"cantar_real_envido", RealEnvidoCommand::build},
        {"cantar_falta_envido", FaltaEnvidoCommand::build},
        {"cantar_truco", TrucoCommand::build},
        {"cantar_re_truco", ReTrucoCommand::build},
        {"cantar_vale_cuatro", ValeCuatroCommand::build},
        {"tirar_carta", TirarCartaCommand::build},
        {"valid_commands", ValidCommandsCommand::build},
};

std::unique_ptr<const Command>
CommandFactory::build(std::string str, std::string name,
                      std::unique_ptr<Truco> &game) {
  std::vector<std::string> words = Lexer::split(str);
  std::string head = words.empty() ? "" : words.front();
  std::unique_ptr<const Command> ret;
  if (map.count(head)) {
    ret = map.at(head)("", name);
  } else if (game != nullptr && truco_map.count(head)) {
    words.erase(words.begin());
    std::string param = words.empty() ? "" : words.front();
    ret = truco_map.at(head)(param, name, game);
  } else {
    ret = MsgCommand::build(str, name);
  }
  return ret;
}
