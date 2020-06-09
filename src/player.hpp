#ifndef PLAYER_HPP_DEFINED
#define PLAYER_HPP_DEFINED

#include "creature.hpp"
#include "item.hpp"
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
  template<Trigger trigger, typename... Args>
  void call_triggers(Args... args);

  int turn_count() const;

  bool is_dead() const { return dead; }

  static constexpr int view_range=9;
};

template<Trigger trigger, typename... Args>
void Player::call_triggers(Args... args) {
  for (Item &item : items)
    if (item.trigger == trigger)
      std::get<Trigger_type<trigger>>(item.effect)(args...);
}

#endif //PLAYER_HPP_DEFINED
