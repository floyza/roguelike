#include "game.hpp"
#include "item.hpp"
#include "map.hpp"
#include "tcod_util.hpp"
#include "player.hpp"
#include "gui.hpp"
#include <iostream>

int main() {
  tcod_rand = std::make_unique<TCODRandom>();
  
  game = std::make_unique<Game>();
  game->generate_map();

  game->you->aquire_item(2);

  while (game->do_turn()) ;
}
