#ifndef MONSTER_HPP_DEFINED
#define MONSTER_HPP_DEFINED

#include <string>
#include "creature.hpp"

class Map;

class Monster : public Creature {
  std::string name_;
public:
  Monster(char icon, const TCODColor &color, Map &parent, const std::string &name, int max_hp, int attack, int x=0, int y=0);
  void do_move() override;
  const std::string &name() { return name_; }
  Map &parent;
};

#endif //MONSTER_HPP_DEFINED
