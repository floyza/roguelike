#include "game.hpp"
#include "map.hpp"
#include "player.hpp"
#include "tcod.hpp"

std::unique_ptr<Game> g;

bool Game::do_turn() {
  if (!TCODConsole::isWindowClosed()) {
    TCODConsole::root->clear();
    map->draw();
    you->draw();
    TCODConsole::root->flush();
    TCOD_key_t key;
    TCODSystem::waitForEvent(TCOD_EVENT_KEY_PRESS,&key,nullptr,true);
    switch (key.vk) {
    case TCODK_LEFT:
      if (map->is_walkable(g->you->x-1, g->you->y) || true)
	--g->you->x;
      break;
    case TCODK_RIGHT:
      if (map->is_walkable(g->you->x+1, g->you->y) || true)
	++g->you->x;
      break;
    case TCODK_UP:
      if (map->is_walkable(g->you->x, g->you->y-1) || true)
	--g->you->y;
      break;
    case TCODK_DOWN:
      if (map->is_walkable(g->you->x, g->you->y+1) || true)
	++g->you->y;
      break;
    default: break;
    }
    return true;
  }
  return false;
}
