#include "player.hpp"
#include "game.hpp"
#include "map.hpp"
#include "gui.hpp"
#include "monster.hpp"
#include "item.hpp"
#include <cassert>

Player::Player(char icon, const TCODColor &color, int max_hp, int attack, int x, int y)
  : Creature(icon, color, max_hp, attack, x, y)
{
}

Player::~Player() = default;

void Player::do_move() {
  bool moving = true;
  while (!TCODConsole::isWindowClosed() && moving) {
    TCOD_key_t key;
    TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS,&key,nullptr,true);
    int new_x=x;
    int new_y=y;
    switch (key.vk) {
    case TCODK_KP1:
      if (game->map->is_walkable(x-1, y+1)) {
	--new_x;
	++new_y;
      }
      break;
    case TCODK_KP2:
      if (game->map->is_walkable(x, y+1)) {
	++new_y;
      }
      break;
    case TCODK_KP3:
      if (game->map->is_walkable(x+1, y+1)) {
	++new_x;
	++new_y;
      }
      break;
    case TCODK_KP4:
      if (game->map->is_walkable(x-1, y)) {
	--new_x;
      }
      break;
    case TCODK_KP5:
      moving = false;
      break;
    case TCODK_KP6:
      if (game->map->is_walkable(x+1, y)) {
	++new_x;
      }
      break;
    case TCODK_KP7:
      if (game->map->is_walkable(x-1, y-1)) {
	--new_x;
	--new_y;
      }
      break;
    case TCODK_KP8:
      if (game->map->is_walkable(x, y-1)) {
	--new_y;
      }
      break;
    case TCODK_KP9:
      if (game->map->is_walkable(x+1, y-1)) {
	++new_x;
	--new_y;
      }
      break;
    case TCODK_CHAR:
      switch (key.c) {
      case 'r':
	for (int x=0; x < game->map->get_width(); ++x) {
	  for (int y=0; y< game->map->get_height(); ++y) {
	    for (int x_adj = x-1; x_adj <= x+1; ++x_adj) {
	      for (int y_adj = y-1; y_adj <= y+1; ++y_adj) {
		if (x_adj < 0 || game->map->get_width() <= x_adj || y_adj < 0 || game->map->get_height() <= y_adj)
		  continue;
		if (game->map->is_walkable(x_adj, y_adj)) {
		  game->map->tile(x,y).discovered=true;
		  break;
		}
	      }
	    }
	  }
	}
	break;
      case 'u':
	for (int x=1; x<game->map->get_width()-1; ++x) {
	  for (int y=1; y<game->map->get_height()-1; ++y) {
	    game->map->tile(x,y).discovered=false;
	  }
	}
	break;
      case 's':
	TCODSystem::saveScreenshot("screenshot.png");
	break;
      }
    default: break;
    }
    if (new_x!=x || new_y!=y) {
      bool attacked=false;
      for (Monster &mon : game->map->monsters) {
	if (mon.x == new_x && mon.y == new_y) {
	  do_attack(mon);
	  attacked=true;
	  break;
	}
      }
      if (!attacked) {
	x = new_x;
	y = new_y;
	call_triggers<Trigger::ON_MOVE>();
      }
      moving = false;
    }
  }
  call_triggers<Trigger::ON_TURN>();
  ++total_turns;
}

void Player::do_attack(Creature &target) {
  do_attack_sans_triggers(target);
  call_triggers<Trigger::ON_HIT>(std::ref(target));
}

void Player::do_attack_sans_triggers(Creature &target) {
  int damage = attack;
  call_triggers<Trigger::DAM_MOD>(std::ref(damage), std::ref(target));
  target.take_damage(damage, *this);
}

void Player::take_damage(int amount, Player &source) {
  call_triggers<Trigger::DAM_REDUCE>(std::ref(amount), std::ref(source));
  game->send_msg({"You hit yourself for " + std::to_string(amount) + " damage!"});
  hp -= amount;
  if (hp<=0)
    die();
  call_triggers<Trigger::ON_DAM>(source);
}

void Player::take_damage(int amount, Monster &source) {
  call_triggers<Trigger::DAM_REDUCE>(std::ref(amount), std::ref(source));
  game->send_msg({"The " + source.name() + " attacks you for " + std::to_string(amount) + " damage!"});
  hp -= amount;
  if (hp<=0)
    die();
  call_triggers<Trigger::ON_DAM>(std::ref(source));
}

void Player::take_damage(int amount) {
  Monster dummy = Monster{"dummy", *game->map};
  call_triggers<Trigger::DAM_REDUCE>(std::ref(amount), std::ref(dummy));/*quick fix, TODO: probably have an actual source*/
  hp -= amount;
  if (hp<=0)
    die();  
  call_triggers<Trigger::ON_DAM>(std::ref(dummy));
}

void Player::die() {
  game->send_msg({"You are dead...", TCODColor::red});
  dead = true;
};

void Player::aquire(const std::string &id) {
  items.emplace_back(id);
}

int Player::turn_count() const {
  return total_turns;
}
