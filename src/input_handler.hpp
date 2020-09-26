#ifndef DEFINED_INPUT_HANDLER_HPP
#define DEFINED_INPUT_HANDLER_HPP

#include "tcod_util.hpp"
#include "command.hpp"
#include "inventory_gui.hpp"
#include <unordered_map>
#include <memory>
#include <vector>

class Command;
class Player;

class Input_handler {
public:
  virtual ~Input_handler() { };
  virtual std::unique_ptr<Command> get_input()=0;
};

class Move_input_handler {
protected:
  Move_input_handler(Creature &target);
  Move_command move_up_;
  Move_command move_down_;
  Move_command move_right_;
  Move_command move_left_;
  Move_command move_nw_;
  Move_command move_ne_;
  Move_command move_sw_;
  Move_command move_se_;
  Null_command null_command_;
};

class Stairs_input_handler {
  protected:
    Stairs_input_handler(Player &target);
    Stairs_move_command move_upstairs_;
    Stairs_move_command move_downstairs_;
};

class Player_input_handler : protected Move_input_handler, protected Stairs_input_handler, public Input_handler {
  std::unordered_map<TCOD_key_t, Command *> buttons_;
  std::vector<std::unique_ptr<Command>> commands_;
public:
  Player_input_handler(Player &player);
  std::unique_ptr<Command> get_input() override;
  std::unique_ptr<Command> handle_key(const TCOD_key_t &input);
};

class Inventory_input_handler : public Input_handler {
  private:
    Inventory_gui gui;
  public:
    Inventory_input_handler(Player &player, std::function<void()> close_callback);
    std::unique_ptr<Command> get_input() override;
};

class Monster_input_handler : protected Move_input_handler, public Input_handler {
  Creature &monster;
  Creature &target;
  Pos dest {-1,-1};
  Pos step_to_dest();
public:
  Monster_input_handler(Creature &monster);
  std::unique_ptr<Command> get_input() override;
};

class Lua_input_handler : public Input_handler {
public:
  std::unique_ptr<Command> get_input() override {/*do nothing*/};
  //Command &handle_input(const TCOD_key_t &input);
};

TCOD_key_t tcod_key_of_char(char c);

#endif
