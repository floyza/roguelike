#include "game.hpp"
#include "map.hpp"
#include "tcod_util.hpp"
#include "player.hpp"
#include "gui.hpp"
#include <iostream>

int main() {
  tcod_rand = std::make_unique<TCODRandom>();
  
  game = std::make_unique<Game>();
  game->init_lua();
  game->generate_map();

  game->you->aquire("test");

  while (game->do_turn()) ;
}
