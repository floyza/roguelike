#include "creature.hpp"
#include <libtcod/libtcod.hpp>

Creature::Creature(char icon, const TCODColor &color, int max_hp, int attack, int x, int y)
  : icon(icon), color(color), max_hp(max_hp), hp(max_hp), attack(attack), x(x), y(y)
{
}

void Creature::do_attack(Creature &target) {
  target.take_damage(attack, *this);
}

void Creature::take_damage(int amount, Creature &source) {
  hp -= amount;
}

void Creature::draw() const {
  TCODConsole::root->setCharForeground(x,y,color);
  TCODConsole::root->setChar(x,y,icon);
}

Creature::~Creature() { }
