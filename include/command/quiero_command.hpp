#pragma once

#include "../../include/truco_domain_engine.hpp"
#include "command.hpp"
#include <memory>
#include <string>

class QuieroCommand : public Command {
public:
  QuieroCommand(std::string, std::unique_ptr<Truco> &);
  virtual ~QuieroCommand() override = default;

  virtual CommandResponse execute() const override;

  static std::unique_ptr<const Command> build(std::string, std::string,
                                              std::unique_ptr<Truco> &);

private:
  std::string name;
  std::unique_ptr<Truco> &game;
};
