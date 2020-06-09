#ifndef DEFINED_ITEM_HPP
#define DEFINED_ITEM_HPP

#include <functional>
#include <string>
#include <variant>

class Creature;

enum class Trigger { DAM_MOD, DAM_REDUCE, ON_HIT, ON_KILL, ON_MOVE, ON_TURN, ON_DAM };

class Item {
public:
  typedef std::function<void(void)> generic_func;
  typedef std::function<void(Creature &)> target_generic_func;
  typedef std::function<void(int &)> modify_func;
  typedef std::function<void(int &,Creature &)> target_modify_func;
  Item(const std::string &id);
  Item(const std::string &id, int x, int y);

  // template<typename Args...>
  // call_effect(Args... args);
  std::variant<modify_func, generic_func, target_modify_func, target_generic_func> effect;

  Trigger trigger;
  std::string name;
  void draw() const;
  int x,y;
  ~Item();
};

template<typename T>
constexpr bool is_trigger_type(Trigger trigger) {
  switch (trigger) {
  case Trigger::ON_MOVE: case Trigger::ON_TURN:
    return std::is_same<T,Item::generic_func>::value;
  case Trigger::ON_HIT: case Trigger::ON_KILL: case Trigger::ON_DAM:
    return std::is_same<T,Item::target_generic_func>::value;
  case Trigger::DAM_MOD: case Trigger::DAM_REDUCE:
    return std::is_same<T,Item::target_modify_func>::value;
  }
  throw std::runtime_error{"is_type<T>(Trigger): invalid trigger"};
}

#endif //DEFINED_ITEM_HPP
