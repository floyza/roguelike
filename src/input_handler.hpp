#ifndef DEFINED_INPUT_HANDLER_HPP
#define DEFINED_INPUT_HANDLER_HPP

#include "tcod_util.hpp"
#include "command.hpp"
#include <unordered_map>

class Command;
class Player;

class Input_hander {
public:
  virtual ~Input_hander() { };
  virtual Command &get_input()=0;
};

class Default_input_handler : public Input_hander {
  Move_command move_up_;
  Move_command move_down_;
  Move_command move_right_;
  Move_command move_left_;
  Move_command move_nw_;
  Move_command move_ne_;
  Move_command move_sw_;
  Move_command move_se_;
  Null_command null_command_;
  std::unordered_map<TCOD_key_t, Command *> buttons_;
public:
  Default_input_handler(Player &target);
  Command &get_input() override;
  Command &handle_input(const TCOD_key_t &input);
};

class Lua_input_handler : public Input_hander {
public:
  Command &get_input() override {/*do nothing*/};
  //Command &handle_input(const TCOD_key_t &input);
};

TCOD_key_t tcod_key_of_char(char c);

#endif
