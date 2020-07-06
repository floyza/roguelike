#ifndef DEFINED_LUA_HPP
#define DEFINED_LUA_HPP

#include <string>
#include <memory>
#include <map>
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

class Lua_manager {
  sol::state lua_state;

  std::vector<Lua_item> item_generators;
  std::vector<Lua_monster> monster_generators;
  std::vector<Lua_status> status_generators;

  std::map<std::string, Lua_item *> item_name_map;
  std::map<std::string, Lua_monster *> monster_name_map;
  std::map<std::string, Lua_status *> status_name_map;

  std::multimap<int, Lua_item *> item_rarity_map;
  std::multimap<int, Lua_monster *> monster_rarity_map;
public:
  Lua_manager();

  void load_file(const std::string &file);

  sol::function get_func(const std::string &func);

  const Lua_item &get_item(const std::string &id) const;
  const Lua_monster &get_mon(const std::string &id) const;
  const Lua_status &get_status(const std::string &id) const;

  const Lua_item &get_rand_item(int depth) const;
  const Lua_monster &get_rand_mon(int depth) const;

};

#endif //DEFINED_LUA_HPP
