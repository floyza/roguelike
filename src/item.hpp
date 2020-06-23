#ifndef DEFINED_ITEM_HPP
#define DEFINED_ITEM_HPP

#include <string>

#include "effect.hpp"

class Item {
  Effect effect;
public:
  Item(const std::string &id);
  Item(const std::string &id, int x, int y);

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
