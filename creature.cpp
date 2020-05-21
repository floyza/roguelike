#include "creature.hpp"
#include "tcod.hpp"

Creature::Creature(char icon, int x, int y)
  : icon(icon), x(x), y(y)
{
}

void Creature::draw() {
  TCODConsole::root->setChar(x,y,icon);
}
