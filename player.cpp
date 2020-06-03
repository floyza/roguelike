#include "player.hpp"
#include "game.hpp"
#include "map.hpp"
#include "gui.hpp"
#include "monster.hpp"

Player::Player(char icon, const TCODColor &color, int max_hp, int attack, int x, int y)
  : Creature(icon, color, max_hp, attack, x, y)
{
}

void Player::do_move() {
  while (!TCODConsole::isWindowClosed()) {
    TCOD_key_t key;
    TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS,&key,nullptr,true);
    int new_x=x;
    int new_y=y;
    switch (key.vk) {
    case TCODK_KP1:
      if (g->map->is_walkable(x-1, y+1)) {
	--new_x;
	++new_y;
      }
      break;
    case TCODK_KP2:
      if (g->map->is_walkable(x, y+1)) {
	++new_y;
      }
      break;
    case TCODK_KP3:
      if (g->map->is_walkable(x+1, y+1)) {
	++new_x;
	++new_y;
      }
      break;
    case TCODK_KP4:
      if (g->map->is_walkable(x-1, y)) {
	--new_x;
      }
      break;
    case TCODK_KP5:
      return;
      break;
    case TCODK_KP6:
      if (g->map->is_walkable(x+1, y)) {
	++new_x;
      }
      break;
    case TCODK_KP7:
      if (g->map->is_walkable(x-1, y-1)) {
	--new_x;
	--new_y;
      }
      break;
    case TCODK_KP8:
      if (g->map->is_walkable(x, y-1)) {
	--new_y;
      }
      break;
    case TCODK_KP9:
      if (g->map->is_walkable(x+1, y-1)) {
	++new_x;
	--new_y;
      }
      break;
    case TCODK_CHAR:
      switch (key.c) {
      case 'r':
	for (int x=0; x < g->map->get_width(); ++x) {
	  for (int y=0; y< g->map->get_height(); ++y) {
	    for (int x_adj = x-1; x_adj <= x+1; ++x_adj) {
	      for (int y_adj = y-1; y_adj <= y+1; ++y_adj) {
		if (x_adj < 0 || g->map->get_width() <= x_adj || y_adj < 0 || g->map->get_height() <= y_adj)
		  continue;
		if (g->map->is_walkable(x_adj, y_adj)) {
		  g->map->tile(x,y).discovered=true;
		  break;
		}
	      }
	    }
	  }
	}
	break;
      case 'u':
	for (int x=1; x<g->map->get_width()-1; ++x) {
	  for (int y=1; y<g->map->get_height()-1; ++y) {
	    g->map->tile(x,y).discovered=false;
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
      for (std::unique_ptr<Monster> &mon : g->map->monsters) {
	if (mon->x == new_x && mon->y == new_y) {
	  do_attack(*mon);
	  attacked=true;
	  break;
	}
      }
      if (!attacked) {
	x = new_x;
	y = new_y;
      }
      break;
    }
  }
}

void Player::do_attack(Creature &target) {
  target.take_damage(attack, *this);
}

void Player::take_damage(int amount, Player &source) {
  g->msg_log->send_msg({"You damage yourself!"});
  g->msg_log->send_nl();
  hp -= amount;
}

void Player::take_damage(int amount, Monster &source) {
  g->msg_log->send_msg({"The " + source.name() + " attacks you!"});
  g->msg_log->send_nl();
  hp -= amount;
}
