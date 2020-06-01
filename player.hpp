#ifndef PLAYER_HPP_DEFINED
#define PLAYER_HPP_DEFINED

#include "creature.hpp"

class Player : public Creature {
public:
  using Creature::Creature;
  void do_move() override;
  static constexpr int view_range=9;
};

#endif //PLAYER_HPP_DEFINED
