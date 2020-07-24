#ifndef DEFINED_INPUT_HANDLER_HPP
#define DEFINED_INPUT_HANDLER_HPP

#include "tcod_util.hpp"
#include "command.hpp"
#include <unordered_map>

class Command;
class Player;

class Input_handler {
public:
  virtual ~Input_hander() { };
  virtual Command &get_input()=0;
};

/** Helper for Input_handlers that are used for movement
 */
class Move_input_handler : public Input_handler {
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

class Player_input_handler : public Move_input_handler {
  std::unordered_map<TCOD_key_t, Command *> buttons_;
public:
  Player_input_handler(Creature &target);
  Command &get_input() override;
  Command &handle_key(const TCOD_key_t &input);
};

class Monster_input_handler : public Move_input_handler {
  Creature &target;
public:
  Monster_input_handler(Creature &target);
  Command &get_input() override;
};

class Lua_input_handler : public Input_handler {
public:
  Command &get_input() override {/*do nothing*/};
  //Command &handle_input(const TCOD_key_t &input);
};

TCOD_key_t tcod_key_of_char(char c);

#endif
