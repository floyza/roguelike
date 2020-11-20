#ifndef PLAYER_HPP_DEFINED
#define PLAYER_HPP_DEFINED

#include "creature.hpp"
#include "item.hpp"
#include "status.hpp"
#include <vector>
#include <string>
#include <stack>

class Monster;
class Item;
class Status;
class Input_handler;
enum class Trigger;

struct Inven_item {
  Item item;
  bool equipped=false;
};

class Player : public Creature {
  private:
    void take_damage(int amount, Player &source) override;
    void take_damage(int amount, Monster &source) override;
    bool dead=false;
    int energy=0;
    int speed=100;
    std::vector<Inven_item> items;
    std::vector<Status> statuses;
    int total_turns=0;
    std::stack<std::unique_ptr<Input_handler>> inputs;
    std::unique_ptr<Input_handler> &current_input() { return inputs.top(); }
  public:
    Player(char icon, const TCODColor &color, int max_hp, int attack, const Pos &pos = {0, 0});
    ~Player();

    bool do_move(const Pos &new_pos) override;
    bool do_turn() override;
    void gain_energy();
    void do_attack(Creature &target, bool triggers=true);
    void die() override;

    Faction faction() const override { return Faction::Player; }

    void take_damage(int amount);

    void aquire_item(int id);
    void aquire_item(const std::string &name);
    void aquire_item(const Lua_item &base);
    void aquire_item(const Item &item);

    void aquire_status(int id);
    void aquire_status(const std::string &name);
    void aquire_status(const Lua_status &base);
    void remove_status(int id);
    void remove_status(const std::string &name);

    int inven_size() const;
    Inven_item &inven_item(int i);
    const Inven_item &inven_item(int i) const;

    void push_input_handler(std::unique_ptr<Input_handler> input);
    void pop_input_handler(); // used by Input_handlers to remove themselves

    template<Trigger trigger, typename... Args>
    void call_triggers(Args... args);

    int turn_count() const;

    bool is_dead() const { return dead; }
};

template<Trigger trigger, typename... Args>
void Player::call_triggers(Args... args) {
  for (auto &item : items)
    if (item.equipped)
      if (item.item.get_trigger() == trigger)
        item.item.call_effect<trigger>(args...);
  for (Status &status : statuses)
    if (status.get_trigger() == trigger)
      status.call_effect<trigger>(args...);
}

#endif //PLAYER_HPP_DEFINED
