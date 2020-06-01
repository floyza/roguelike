#include "player.hpp"
#include "game.hpp"
#include "map.hpp"

Player::Player(char icon, const TCODColor &color, int max_hp, int attack, int x, int y)
  : Creature(icon, color, max_hp, attack, x, y)
{
}

void Player::do_move() {
  TCOD_key_t key;
  TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS,&key,nullptr,true);
  switch (key.vk) {
  case TCODK_KP1:
    if (g->map->is_walkable(g->you->x-1, g->you->y+1)) {
      --g->you->x;
      ++g->you->y;
    }
    break;
  case TCODK_KP2:
    if (g->map->is_walkable(g->you->x, g->you->y+1)) {
      ++g->you->y;
    }
    break;
  case TCODK_KP3:
    if (g->map->is_walkable(g->you->x+1, g->you->y+1)) {
      ++g->you->x;
      ++g->you->y;
    }
    break;
  case TCODK_KP4:
    if (g->map->is_walkable(g->you->x-1, g->you->y)) {
      --g->you->x;
    }
    break;
  case TCODK_KP5:
    break;
  case TCODK_KP6:
    if (g->map->is_walkable(g->you->x+1, g->you->y)) {
      ++g->you->x;
    }
    break;
  case TCODK_KP7:
    if (g->map->is_walkable(g->you->x-1, g->you->y-1)) {
      --g->you->x;
      --g->you->y;
    }
    break;
  case TCODK_KP8:
    if (g->map->is_walkable(g->you->x, g->you->y-1)) {
      --g->you->y;
    }
    break;
  case TCODK_KP9:
    if (g->map->is_walkable(g->you->x+1, g->you->y-1)) {
      ++g->you->x;
      --g->you->y;
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
}
