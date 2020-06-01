#ifndef CREATURE_HPP_DEFINED
#define CREATURE_HPP_DEFINED

#include <libtcod/libtcod.hpp>

class Creature {
  char icon;
  TCODColor color;
protected:
  int max_hp;
  int hp;
  int attack;
public:
  Creature(char icon, const TCODColor &color, int max_hp, int attack, int x=0, int y=0);
  void draw() const;
  virtual void do_move()=0;
  void do_attack(Creature &target);
  void take_damage(int amount, Creature &source);
  virtual ~Creature()=0;
  int x,y;
};

#endif //CREATURE_HPP_DEFINED
