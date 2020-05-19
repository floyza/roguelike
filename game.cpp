#include "game.hpp"
#include "map.hpp"
#include "display.hpp"
#include "player.hpp"
#include "tcod.hpp"

std::unique_ptr<Game> g;

bool Game::do_turn() {
  if (!TCODConsole::isWindowClosed()) {
    win->clear();
    win->draw(map->get_drawable());
    win->flush();
    TCOD_key_t key;
    TCODSystem::checkForEvent(TCOD_EVENT_KEY_PRESS,&key,NULL);
    return true;
  }
  return false;
}
