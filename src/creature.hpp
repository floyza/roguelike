#ifndef CREATURE_HPP_DEFINED
#define CREATURE_HPP_DEFINED

#include <libtcod/libtcod.hpp>
#include "factions.hpp"
#include "pos.hpp"

class Player;
class Monster;

class Creature {
  char icon;
  TCODColor color;
protected:
  int max_hp;
  int hp;
  int attack;
public:
  Creature(char icon, const TCODColor &color, int max_hp, int attack, const Pos &pos = {0,0});
  void draw() const;
  int get_hp() const;
  int get_attack() const;
  virtual void do_turn()=0;
  virtual void take_damage(int amount, Player &source)=0;
  virtual void take_damage(int amount, Monster &source)=0;
  virtual bool do_move(const Pos& new_pos)=0;
  virtual Faction faction() const=0;
  virtual void die()=0;
  virtual ~Creature()=0;
  Pos pos;
};

#endif //CREATURE_HPP_DEFINED
