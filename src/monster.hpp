#ifndef MONSTER_HPP_DEFINED
#define MONSTER_HPP_DEFINED

#include <string>
#include "creature.hpp"

class Map;
class Player;
struct Lua_monster;

class Monster : public Creature {
  std::string name_;
  void take_damage(int amount, Player &source) override;
  void take_damage(int amount, Monster &source) override;
  int dest_x=-1, dest_y=-1;
  std::pair<int, int> step_to_dest();
public:
  Monster(const std::string &id, Map &parent, int x=0, int y=0);
  Monster(const Lua_monster &base, Map &parent, int x=0, int y=0);
  ~Monster();

  const std::string &name() { return name_; }

  void do_move() override;
  void do_attack(Creature &target);
  void die() override;

  Map *parent;
};

#endif //MONSTER_HPP_DEFINED
