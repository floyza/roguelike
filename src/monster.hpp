#ifndef MONSTER_HPP_DEFINED
#define MONSTER_HPP_DEFINED

#include <string>
#include "creature.hpp"

class Map;
class Player;
struct Lua_monster;

class Monster : public Creature {
  std::string name_;
  int id_;
  void take_damage(int amount, Player &source) override;
  void take_damage(int amount, Monster &source) override;
  Pos dest = { -1, -1 };
  Pos step_to_dest();
public:
  Monster(int id, Map &parent, const Pos &pos = {0, 0});
  Monster(const std::string &name, Map &parent, const Pos &pos = {0, 0});
  Monster(const Lua_monster &base, Map &parent, const Pos &pos = {0, 0});
  ~Monster();

  int id() const;
  const std::string &name() const;

  void do_turn() override;
  void do_attack(Creature &target);
  void die() override;

  Faction faction() const override { return Faction::Monster; }

  bool do_move(const Pos &new_pos) override;

  Map *parent;
};

#endif //MONSTER_HPP_DEFINED
