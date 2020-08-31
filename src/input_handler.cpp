#include "input_handler.hpp"
#include "tcod_util.hpp"
#include "command.hpp"
#include "player.hpp"
#include "dir.hpp"
#include "game.hpp"
#include "map.hpp"
#include "monster.hpp"
#include <cctype>

Move_input_handler::Move_input_handler(Creature &target)
  : move_up_{target,Dir::n}, move_down_{target,Dir::s}, move_right_{target,Dir::e}, move_left_{target,Dir::w},
    move_nw_{target,Dir::nw}, move_ne_{target,Dir::ne}, move_sw_{target,Dir::sw}, move_se_{target,Dir::se},
    null_command_{}
{
}

Player_input_handler::Player_input_handler(Creature &player)
    : Move_input_handler{player}
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

std::unique_ptr<Command> Player_input_handler::get_input() {
  TCOD_key_t key;
  TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS,&key,nullptr,true);
  return handle_key(key);
}

std::unique_ptr<Command> Player_input_handler::handle_key(const TCOD_key_t &input) {
  auto it = buttons_.find(input);
  if (it != buttons_.end()) {
    return std::make_unique<Move_command>(*it->second);
  }
  return std::make_unique<Null_command>(null_command_);
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

Monster_input_handler::Monster_input_handler(Creature &monster)
  : Move_input_handler{monster}, monster{monster}, target{*game->you}
{
}

std::unique_ptr<Command> Monster_input_handler::get_input() {
  if (game->map->in_fov(monster.pos, target.pos)) {
    dest = target.pos;
  }

  return std::make_unique<Move_command>(monster, step_to_dest());
}

Pos Monster_input_handler::step_to_dest() {
  Pos step = target.pos;
  if (dest.x != -1 && dest.y != -1) {
    TCODMap map(game->map->get_map()->getWidth(), game->map->get_map()->getHeight());
    map.copy(game->map->get_map());
    for (const Monster &other_monster : game->map->monsters) {
      map.setProperties(other_monster.pos.x, other_monster.pos.y, map.isWalkable(other_monster.pos.x, other_monster.pos.y), false);
    }
    TCODPath path(&map);
    path.compute(monster.pos.x, monster.pos.y, dest.x, dest.y);

    if (!path.isEmpty()) {
      path.get(0, &step.x, &step.y);
    } else {
      // still try to walk towards the player
      TCODPath path_closer(game->map->get_map());
      path_closer.compute(monster.pos.x, monster.pos.y, dest.x, dest.y);
      Pos pstep;
      if (!path_closer.isEmpty()) {
        path_closer.get(0, &pstep.x, &pstep.y);
        step = pstep;
      }
    }
  }
  return target.pos-step;
}
