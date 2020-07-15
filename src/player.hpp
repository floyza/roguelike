#ifndef PLAYER_HPP_DEFINED
#define PLAYER_HPP_DEFINED

#include "creature.hpp"
#include "item.hpp"
#include "status.hpp"
#include <vector>
#include <string>

class Monster;
class Item;
class Status;
enum class Trigger;

class Player : public Creature {
  void take_damage(int amount, Player &source) override;
  void take_damage(int amount, Monster &source) override;
  bool dead=false;
  std::vector<Item> items;
  std::vector<Status> statuses;
  int total_turns=0;
public:
  Player(char icon, const TCODColor &color, int max_hp, int attack, int x=0, int y=0);
  ~Player();

  void do_move() override;
  void do_attack(Creature &target);
  void do_attack_sans_triggers(Creature &target); // still calls DAM_MOD triggers
  void die() override;

  void take_damage(int amount);

  void aquire_item(int id);
  void aquire_item(const Lua_item &base);

  void aquire_status(int id);
  void aquire_status(const Lua_status &base);
  void remove_status(int id);

  template<Trigger trigger, typename... Args>
  void call_triggers(Args... args);

  int turn_count() const;

  bool is_dead() const { return dead; }

  static constexpr int view_range=9;
};

template<Trigger trigger, typename... Args>
void Player::call_triggers(Args... args) {
  for (Item &item : items)
    if (item.get_trigger() == trigger)
      item.call_effect<trigger>(args...);
  for (Status &status : statuses)
    if (status.get_trigger() == trigger)
      status.call_effect<trigger>(args...);
}

#endif //PLAYER_HPP_DEFINED
