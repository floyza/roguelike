#ifndef PLAYER_HPP_DEFINED
#define PLAYER_HPP_DEFINED

#include "creature.hpp"
#include <vector>
#include <string>

class Monster;
class Item;
enum class Trigger;

class Player : public Creature {
  void take_damage(int amount, Player &source);
  void take_damage(int amount, Monster &source);
  bool dead=false;
  std::vector<Item> items;
public:
  Player(char icon, const TCODColor &color, int max_hp, int attack, int x=0, int y=0);
  ~Player();

  void do_move() override;
  void do_attack(Creature &target) override;
  void die() override;

  void aquire(const std::string &id);
  void call_triggers_generic(const Trigger &trigger);
  int call_triggers_mod(const Trigger &trigger, int arg);

  bool is_dead() const { return dead; }

  static constexpr int view_range=9;
};

#endif //PLAYER_HPP_DEFINED
