#include "input_handler.hpp"
#include "tcod_util.hpp"
#include "command.hpp"
#include "player.hpp"
#include "dir.hpp"
#include <cctype>

Move_input_handler::Move_input_handler(Creature &target)
  : move_up_{target,Dir::n}, move_down_{target,Dir::s}, move_right_{target,Dir::e}, move_left_{target,Dir::w},
    move_nw_{target,Dir::nw}, move_ne_{target,Dir::ne}, move_sw_{target,Dir::sw}, move_se_{target,Dir::se},
    null_command_{}
{
}

Player_input_handler::Player_input_handler(Creature &target)
    : Move_input_handler{target}
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

Command &Player_input_handler::get_input() {
  TCOD_key_t key;
  TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS,&key,nullptr,true);
  return handle_key(key);
}

Command &Player_input_handler::handle_key(const TCOD_key_t &input) {
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

Monster_input_handler::Monster_input_handler(Creature &target)
  : Move_input_handler{target}, target{target}
{
}

Command &Monster_input_handler::get_input() {
}
