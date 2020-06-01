#ifndef CREATURE_HPP_DEFINED
#define CREATURE_HPP_DEFINED

#include <libtcod/libtcod.hpp>

class Creature {
  char icon;
public:
  Creature(char icon, const TCODColor &color, int x=0, int y=0);
  int x,y;
  TCODColor color;
  void draw() const;
  virtual void do_move()=0;
  virtual ~Creature()=0;
};

#endif //CREATURE_HPP_DEFINED
