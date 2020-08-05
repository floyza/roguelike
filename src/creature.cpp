#include "creature.hpp"
#include <libtcod/libtcod.hpp>

Creature::Creature(char icon, const TCODColor &color, int max_hp, int attack, const Pos &pos)
  : icon(icon), color(color), max_hp(max_hp), hp(max_hp), attack(attack), pos(pos)
{
}

void Creature::draw() const {
  TCODConsole::root->setCharForeground(pos.x,pos.y,color);
  TCODConsole::root->setChar(pos.x,pos.y,icon);
}

int Creature::get_hp() const {
  return hp;
}

int Creature::get_attack() const {
  return attack;
}

Creature::~Creature() { }
