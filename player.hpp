#ifndef PLAYER_HPP_DEFINED
#define PLAYER_HPP_DEFINED

#include "creature.hpp"

class Monster;

class Player : public Creature {
  void take_damage(int amount, Player &source);
  void take_damage(int amount, Monster &source);
public:
  Player(char icon, const TCODColor &color, int max_hp, int attack, int x=0, int y=0);
  void do_move() override;

  void do_attack(Creature &target);

  static constexpr int view_range=9;
};

#endif //PLAYER_HPP_DEFINED
