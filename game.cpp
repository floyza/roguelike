#include "game.hpp"
#include "map.hpp"
#include "player.hpp"
#include "tcod.hpp"

std::unique_ptr<Game> g;

bool Game::do_turn() {
  if (!TCODConsole::isWindowClosed()) {
    TCOD_key_t key;
    TCODConsole::root->clear();
    map->draw();
    you->draw();
    TCODConsole::root->flush();
    TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS,&key,nullptr,true);
    switch (key.vk) {
    case TCODK_KP1:
      if (map->is_walkable(you->x-1, you->y+1)) {
	--you->x;
	++you->y;
      }
      break;
    case TCODK_KP2:
      if (map->is_walkable(you->x, you->y+1)) {
	++you->y;
      }
      break;
    case TCODK_KP3:
      if (map->is_walkable(you->x+1, you->y+1)) {
	++you->x;
	++you->y;
      }
      break;
    case TCODK_KP4:
      if (map->is_walkable(you->x-1, you->y)) {
	--you->x;
      }
      break;
    case TCODK_KP5:
      break;
    case TCODK_KP6:
      if (map->is_walkable(you->x+1, you->y)) {
	++you->x;
      }
      break;
    case TCODK_KP7:
      if (map->is_walkable(you->x-1, you->y-1)) {
	--you->x;
	--you->y;
      }
      break;
    case TCODK_KP8:
      if (map->is_walkable(you->x, you->y-1)) {
	--you->y;
      }
      break;
    case TCODK_KP9:
      if (map->is_walkable(you->x+1, you->y-1)) {
	++you->x;
	--you->y;
      }
      break;
    case TCODK_CHAR:
      switch (key.c) {
      case 'r':
	for (int x=0; x<map->get_width(); ++x) {
	  for (int y=0; y<map->get_height(); ++y) {
	    for (int x_adj = x-1; x_adj <= x+1; ++x_adj) {
	      for (int y_adj = y-1; y_adj <= y+1; ++y_adj) {
		if (x_adj < 0 || map->get_width() <= x_adj || y_adj < 0 || map->get_height() <= y_adj)
		  continue;
		if (map->is_walkable(x_adj, y_adj)) {
		  map->tile(x,y).discovered=true;
		  break;
		}
	      }
	    }
	  }
	}
	break;
      case 'u':
	for (int x=1; x<map->get_width()-1; ++x) {
	  for (int y=1; y<map->get_height()-1; ++y) {
	    map->tile(x,y).discovered=false;
	  }
	}
	break;
      case 's':
	TCODSystem::saveScreenshot("screenshot.png");
	break;
      }
    default: break;
    }
    return true;
  }
  return false;
}
