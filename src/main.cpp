#include "game.hpp"
#include "gui.hpp"
#include "item.hpp"
#include "map.hpp"
#include "player.hpp"
#include "tcod_util.hpp"
#include <iostream>

int main() {
  tcod_rand = std::make_unique<TCODRandom>();

  game = std::make_unique<Game>();
  game->start();

  game->you->aquire_item("Test Item");

  game->play();
}
