#include "game.hpp"
#include "map.hpp"
#include "display.hpp"
#include "player.hpp"
#include "tcod.hpp"

int main() {
  int x=80,y=50;
  g = std::make_unique<Game>();
  g->win = std::make_unique<Display>(x,y,"roguelike");
  g->you = std::make_unique<Player>();

  tcod_rand = std::make_unique<TCODRandom>();

  Map map(x,y,0);
  g->map = &map;

  while (g->do_turn()) ;
}
