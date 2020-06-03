#ifndef DEFINED_MON_ID_HPP
#define DEFINED_MON_ID_HPP

#include <libtcod/libtcod.hpp>
#include <string>

struct mon_id {
  char icon;
  TCODColor color;
  std::string name;
  int max_hp;
  int attack;
};

#endif //DEFINED_MON_ID_HPP
