#ifndef MONSTER_HPP_DEFINED
#define MONSTER_HPP_DEFINED

#include "creature.hpp"

class Map;

class Monster : public Creature {
public:
  Monster(char icon, const TCODColor &color, Map &parent, int max_hp, int attack, int x=0, int y=0);
  void do_move() override;
  Map &parent;
};

#endif //MONSTER_HPP_DEFINED
