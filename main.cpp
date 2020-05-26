#include "game.hpp"
#include "map.hpp"
#include "player.hpp"
#include "tcod.hpp"
#include <iostream>

int main() {
  int x=120,y=50;
  g = std::make_unique<Game>();
  g->you = std::make_unique<Player>('@');

  tcod_rand = std::make_unique<TCODRandom>();

  Map map(x,y,0);
  g->map = &map;

  TCODConsole::setCustomFont("font.png", TCOD_FONT_LAYOUT_ASCII_INROW | TCOD_FONT_TYPE_GREYSCALE);
  TCODConsole::initRoot(x,y,"Roguelike");
  while (g->do_turn()) ;
}
