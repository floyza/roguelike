#ifndef DEFINED_COMMAND_HPP
#define DEFINED_COMMAND_HPP

#include "pos.hpp"

class Creature;

class Command {
public:
  virtual ~Command() { };
  virtual int execute()=0;
};

class Null_command : public Command {
public:
  int execute() override { return 0; }
};

class Move_command : public Command {
  Creature &target;
  Pos pos_;
  int cost_;
public:
  // moves relative to current position
  Move_command(Creature &target, const Pos &pos) : target{target}, pos_{pos} {}
  int execute() override;
};

#endif
