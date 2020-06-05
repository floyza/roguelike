#include "game.hpp"
#include "map.hpp"
#include "tcod_util.hpp"
#include "player.hpp"
#include "gui.hpp"
#include <iostream>

int main() {
  tcod_rand = std::make_unique<TCODRandom>();
  
  g = std::make_unique<Game>();
  g->init_lua();
  g->generate_map();

  while (g->do_turn()) ;
}
