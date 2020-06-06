#ifndef DEFINED_ITEM_HPP
#define DEFINED_ITEM_HPP

#include <functional>
#include <string>
#include <variant>

enum class Trigger { DAM_MOD, DAM_REDUCE, ON_HIT, ON_KILL, ON_MOVE };

bool has_arg(Trigger trigger);

class Item {
public:
  typedef std::function<int(int)> modify_func;
  typedef std::function<void(void)> generic_func;
  Item(const std::string &id);
  Item(const std::string &id, int x, int y);
  std::variant<modify_func, generic_func> effect;
  Trigger trigger;
  std::string name;
  void draw() const;
  int x,y;
  ~Item();
};

#endif //DEFINED_ITEM_HPP
