#ifndef DEFINED_ITEM_HPP
#define DEFINED_ITEM_HPP

#include <string>

#include "effect.hpp"
#include "lua.hpp"
#include "pos.hpp"

struct Lua_item;

class Item {
  Effect effect;
  int id_;
  std::string name_;

public:
  Item(int id, const Pos &pos = {0, 0});
  Item(const std::string &name, const Pos &pos = {0, 0});
  Item(const Lua_item &base, const Pos &pos = {0, 0});

  template <Trigger trigger, typename... Args> void call_effect(Args... args);

  Trigger get_trigger();

  int id() const;
  const std::string &name() const;

  void draw() const;
  Pos pos;
  ~Item();
};

template <Trigger trigger, typename... Args>
void Item::call_effect(Args... args) {
  // we have to take a template argument of our trigger because it needs
  // to be constexpr
  effect.call_effect<trigger>(args...);
}

#endif // DEFINED_ITEM_HPP
