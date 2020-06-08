#ifndef DEFINED_MON_ID_HPP
#define DEFINED_MON_ID_HPP

#include <libtcod/libtcod.hpp>
#include <string>

struct mon_id {
  mon_id() = default;
  mon_id(char icon, const TCODColor &color, const std::string &name, int max_hp, int attack, int rarity)
    : icon(icon), color(color), name(name), max_hp(max_hp), attack(attack), rarity(rarity) { }
  char icon;
  TCODColor color;
  std::string name;
  int max_hp;
  int attack;
  int rarity;
};

#endif //DEFINED_MON_ID_HPP
