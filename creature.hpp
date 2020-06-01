#ifndef CREATURE_HPP_DEFINED
#define CREATURE_HPP_DEFINED

#include <libtcod/libtcod.hpp>

class Creature {
public:
  Creature(char icon, const TCODColor &color, int max_hp, int attack, int x=0, int y=0);
  char icon;
  TCODColor color;
  int max_hp;
  int hp;
  int attack;
  int x,y;
  void draw() const;
  virtual void do_move()=0;
  virtual void do_attack(Creature &target);
  virtual void take_damage(int amount, Creature &source);
  virtual ~Creature()=0;
};

#endif //CREATURE_HPP_DEFINED
