#ifndef DEFINED_COMMAND_HPP
#define DEFINED_COMMAND_HPP

#include <memory>
#include "pos.hpp"

class Creature;
class Player;

class Command {
  public:
    virtual ~Command() { };
    virtual int execute()=0;
    virtual std::unique_ptr<Command> clone()=0;
};

class Null_command : public Command {
  public:
    int execute() override { return 0; }
    std::unique_ptr<Command> clone() override { return std::make_unique<Null_command>(*this); }
};

class Move_command : public Command {
    Creature &target;
    Pos pos_;
  public:
    // moves relative to current position
    Move_command(Creature &target, const Pos &pos) : target{target}, pos_{pos} {}
    int execute() override;
    std::unique_ptr<Command> clone() override { return std::make_unique<Move_command>(*this); }
};

class Stairs_move_command : public Command {
  public:
    enum class Direction { Up, Down };
  private:
    Player &target; // only works on the player
    Direction dir;
  public:
    Stairs_move_command(Player &target, Direction dir)
      : target{target}, dir{dir} {}
    int execute() override;
    std::unique_ptr<Command> clone() override { return std::make_unique<Stairs_move_command>(*this); }
};

#endif
