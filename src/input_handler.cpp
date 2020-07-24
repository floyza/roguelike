#include "input_handler.hpp"
#include "tcod_util.hpp"
#include "command.hpp"
#include "player.hpp"
#include <cctype>

Default_input_handler::Default_input_handler(Player &target)
  : move_up_{target,0,-1}, move_down_{target,0,1}, move_right_{target,1,0}, move_left_{target,-1,0},
    move_nw_{target,-1,-1}, move_ne_{target,1,-1}, move_sw_{target,-1,1}, move_se_{target,1,1},
    null_command_{}
{
  buttons_[tcod_key_of_char('k')] = &move_up_;
  buttons_[tcod_key_of_char('j')] = &move_down_;
  buttons_[tcod_key_of_char('l')] = &move_right_;
  buttons_[tcod_key_of_char('h')] = &move_left_;
  buttons_[tcod_key_of_char('y')] = &move_nw_;
  buttons_[tcod_key_of_char('u')] = &move_ne_;
  buttons_[tcod_key_of_char('b')] = &move_sw_;
  buttons_[tcod_key_of_char('n')] = &move_se_;
}

Command &Default_input_handler::get_input() {
  TCOD_key_t key;
  TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS,&key,nullptr,true);
  return handle_input(key);
}

Command &Default_input_handler::handle_input(const TCOD_key_t &input) {
  auto it = buttons_.find(input);
  if (it != buttons_.end()) {
    return *it->second;
  }
  return null_command_;
}

TCOD_key_t tcod_key_of_char(char c) {
  TCOD_key_t key;
  key.vk = TCODK_CHAR;
  key.c = c;
  key.shift = std::isupper(c);
  key.pressed=true;
  key.lalt=false;
  key.lctrl=false;
  key.ralt=false;
  key.rctrl=false;
  return key;
}
