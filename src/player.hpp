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
  int total_turns=0;
public:
  Player(char icon, const TCODColor &color, int max_hp, int attack, int x=0, int y=0);
  ~Player();

  void do_move() override;
  void do_attack(Creature &target) override;
  void do_attack_sans_triggers(Creature &target); // still calls DAM_MOD triggers
  void die() override;

  void take_damage(int amount);

  void aquire(const std::string &id);
  void call_triggers(const Trigger &trigger); // generic_func
  void call_triggers(const Trigger &trigger, Creature &target); // target_generic_func
  void call_triggers(const Trigger &trigger, int &arg); // modify_func
  void call_triggers(const Trigger &trigger, int &arg, Creature &target); // target_modify_func

  int turn_count() const;

  bool is_dead() const { return dead; }

  static constexpr int view_range=9;
};

#endif //PLAYER_HPP_DEFINED
