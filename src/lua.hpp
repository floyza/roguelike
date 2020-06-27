#ifndef DEFINED_LUA_HPP
#define DEFINED_LUA_HPP

#include <string>
#include <memory>
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

#endif //DEFINED_LUA_HPP
