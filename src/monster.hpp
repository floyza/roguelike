#ifndef MONSTER_HPP_DEFINED
#define MONSTER_HPP_DEFINED

#include "creature.hpp"
#include <string>

class Map;
class Player;
struct Lua_monster;

class Monster : public Creature {
  std::string name_;
  int id_;
  bool dead_ = false;
  void take_damage(int amount, Player &source) override;
  void take_damage(int amount, Monster &source) override;
  Pos dest = {-1, -1};
  Pos step_to_dest();
  int parent_index;
  int energy = 0;
  int speed = 100;

public:
  Monster(int id, int parent, const Pos &pos = {0, 0});
  Monster(const std::string &name, int parent, const Pos &pos = {0, 0});
  Monster(const Lua_monster &base, int parent, const Pos &pos = {0, 0});
  ~Monster();
  Monster(const Monster &) = delete;
  Monster &operator=(const Monster &) = delete;
  Monster(Monster &&) = default;
  Monster &operator=(Monster &&) = default;

  int id() const;
  const std::string &name() const;

  bool do_turn() override;
  void gain_energy() override;
  bool get_energy() const override { return energy; }
  void do_attack(Creature &target);
  void die() override;
  void push_death(); // if we are dead: remove ourselves from existence

  Faction faction() const override { return Faction::Monster; }

  bool do_move(const Pos &new_pos) override;

  Map &parent();
  const Map &parent() const;
};

#endif // MONSTER_HPP_DEFINED
