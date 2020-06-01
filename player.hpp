#ifndef PLAYER_HPP_DEFINED
#define PLAYER_HPP_DEFINED

#include "creature.hpp"

class Player : public Creature {
public:
  Player(char icon, const TCODColor &color, int max_hp, int attack, int x=0, int y=0);
  void do_move() override;
  static constexpr int view_range=9;
};

#endif //PLAYER_HPP_DEFINED
