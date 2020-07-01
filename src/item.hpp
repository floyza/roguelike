#ifndef DEFINED_ITEM_HPP
#define DEFINED_ITEM_HPP

#include <string>

#include "effect.hpp"
#include "lua.hpp"

struct Lua_item;

class Item {
  Effect effect;
public:
  Item(const std::string &id, int x=0, int y=0);
  Item(const Lua_item &base, int x=0, int y=0);

  template<Trigger trigger, typename... Args>
  void call_effect(Args... args);

  Trigger get_trigger();

  std::string name;
  void draw() const;
  int x,y;
  ~Item();
};

template<Trigger trigger, typename... Args>
void Item::call_effect(Args... args) {
  // we have to take a template argument of our trigger because it needs
  // to be constexpr
  effect.call_effect<trigger>(args...);
}

#endif //DEFINED_ITEM_HPP
