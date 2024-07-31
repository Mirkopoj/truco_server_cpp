#pragma once

#include "../../include/truco_domain_engine.hpp"
#include "command.hpp"
#include <memory>
#include <string>

class RealEnvidoCommand : public Command {
public:
  RealEnvidoCommand(std::string, std::unique_ptr<Truco> &);
  virtual ~RealEnvidoCommand() override = default;

  virtual CommandResponse execute() const override;

  static std::unique_ptr<const Command> build(std::string, std::string,
                                              std::unique_ptr<Truco> &);

private:
  std::string name;
  std::unique_ptr<Truco> &game;
};
