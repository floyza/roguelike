#include "creature.hpp"
#include <libtcod/libtcod.hpp>

Creature::Creature(char icon, const TCODColor &color, int x, int y)
  : icon(icon), x(x), y(y), color(color)
{
}

void Creature::draw() const {
  TCODConsole::root->setCharForeground(x,y,color);
  TCODConsole::root->setChar(x,y,icon);
}

Creature::~Creature() { }
