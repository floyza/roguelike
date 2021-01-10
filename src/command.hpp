#ifndef DEFINED_COMMAND_HPP
#define DEFINED_COMMAND_HPP

#include <memory>
#include "pos.hpp"
#include "tcod_util.hpp"

class Creature;
class Player;
class Inventory_gui;
class Lua_gui;

class Command {
  public:
    virtual ~Command() { };
    virtual int execute()=0;
    virtual std::unique_ptr<Command> clone()=0;
};

class Lambda_command : public Command {
  private:
    std::function<int(void)> func;
  public:
    // Uses energy equal to function result
    Lambda_command(std::function<int(void)> f)
      : func(f) {}
    // Uses energy equal to arg
    Lambda_command(std::function<void(void)> f, int energy)
      : Lambda_command([f,energy]{ f(); return energy; }) {}
    int execute() override { return func(); }
    std::unique_ptr<Command> clone() override { return std::make_unique<Lambda_command>(*this); }
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

class Inventory_command : public Command {
  private:
    Inventory_gui &gui;
    TCOD_key_t key;
  public:
    Inventory_command(Inventory_gui &gui, TCOD_key_t key);
    int execute() override;
    std::unique_ptr<Command> clone() override { return std::make_unique<Inventory_command>(*this); }
};

class Lua_command : public Command {
  private:
    Lua_gui &gui;
    TCOD_key_t key;
  public:
    Lua_command(Lua_gui &gui, TCOD_key_t key);
    int execute() override;
    std::unique_ptr<Command> clone() override { return std::make_unique<Lua_command>(*this); }
};

class Pickup_command : public Command {
  private:
    Player &target_;
  public:
    Pickup_command(Player &target)
      : target_{target} {}
    int execute() override;
    std::unique_ptr<Command> clone() override { return std::make_unique<Pickup_command>(*this); }
};

#endif
