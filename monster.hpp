#ifndef MONSTER_HPP_DEFINED
#define MONSTER_HPP_DEFINED

#include <string>
#include "creature.hpp"

class Map;
class Player;

class Monster : public Creature {
  std::string name_;
  void take_damage(int amount, Player &source);
  void take_damage(int amount, Monster &source);
public:
  Monster(char icon, const TCODColor &color, Map &parent, const std::string &name, int max_hp, int attack, int x=0, int y=0);
  void do_move() override;
  const std::string &name() { return name_; }

  void do_attack(Creature &target);

  Map &parent;
};

#endif //MONSTER_HPP_DEFINED
