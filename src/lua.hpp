#ifndef DEFINED_LUA_HPP
#define DEFINED_LUA_HPP

#include <stdexcept>
#include <string>
#include <memory>
#include <map>
#include <libtcod/libtcod.hpp>
#include "sol.hpp"

enum class Trigger;

struct Lua_item {
  int id;
  std::string name;
  Trigger type;
  std::string func;
  int rarity;
};

struct Lua_status {
  int id;
  std::string name;
  Trigger type;
  std::string func;
  int duration;
};

struct Lua_monster {
  int id;
  std::string name;
  char icon;
  TCODColor color;
  int max_hp;
  int attack;
  int rarity;
 };

class Lua_manager {
  sol::state lua_state;

  std::vector<Lua_item> item_generators;
  std::vector<Lua_monster> monster_generators;
  std::vector<Lua_status> status_generators;

  std::multimap<int, Lua_item *> item_rarity_map;
  std::multimap<int, Lua_monster *> monster_rarity_map;

  template<typename T>
  void mandatory(const std::string &table, int id, const std::string &flag, T &member);
  template<typename T>
  void optional(const std::string &table, int id, const std::string &flag, T &member);

  static constexpr const char *item_def_table = "item_definitions";
  static constexpr const char *status_def_table = "status_definitions";
  static constexpr const char *monster_def_table = "monster_definitions";

  Lua_item load_item(int id);
  Lua_status load_status(int id);
  Lua_monster load_monster(int id);
public:
  Lua_manager();

  void init();

  void load_file(const std::string &file);
  void script(const std::string &input);
  void script_cin();

  sol::protected_function get_func(const std::string &func);

  const Lua_item &get_item(int id) const;
  const Lua_item &get_item(const std::string &id) const;
  const Lua_monster &get_mon(int id) const;
  const Lua_monster &get_mon(const std::string &name) const;
  const Lua_status &get_status(int id) const;
  const Lua_status &get_status(const std::string &name) const;

  const Lua_item &get_rand_item(int depth) const;
  const Lua_monster &get_rand_mon(int depth) const;
};

template<typename T>
void Lua_manager::mandatory(const std::string &table, int id, const std::string &flag, T &member) {
  sol::table sol_table = lua_state[table];
  sol::table sol_table2 = sol_table[id];
  member = sol_table2.get<T>(flag);
}

template<typename T>
void Lua_manager::optional(const std::string &table, int id, const std::string &flag, T &member) {
  sol::optional<T> val = lua_state[table][id][flag];
  if (val)
    member = *val;
}


#endif //DEFINED_LUA_HPP
