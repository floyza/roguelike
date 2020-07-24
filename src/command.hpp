#ifndef DEFINED_COMMAND_HPP
#define DEFINED_COMMAND_HPP

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
  int x_, y_;
  int cost_;
public:
  // moves relative to current position
  Move_command(Creature &target, int x, int y) : target{target}, x_{x}, y_{y} { }
  int execute() override;
};

#endif
