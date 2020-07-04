#ifndef DEFINED_LUA_HPP
#define DEFINED_LUA_HPP

#include <string>
#include <memory>
#include <libtcod/libtcod.hpp>
#include "sol.hpp"

enum class Trigger;

struct Lua_item {
  Lua_item();
  Lua_item(Trigger type, const std::string &func, const std::string &name, int rarity);
  Trigger type;
  std::string func;
  std::string name;
  int rarity;
};

struct Lua_status {
  Lua_status();
  Lua_status(Trigger type, const std::string &func, const std::string &name, int duration);
  Trigger type;
  std::string func;
  std::string name;
  int duration;
};

struct Lua_monster {
  Lua_monster();
  Lua_monster(char icon, const TCODColor &color, const std::string &name, int max_hp, int attack, int rarity);
  char icon;
  TCODColor color;
  std::string name;
  int max_hp;
  int attack;
  int rarity;
};

#endif //DEFINED_LUA_HPP
