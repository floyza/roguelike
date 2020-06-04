#ifndef DEFINED_ITEM_HPP
#define DEFINED_ITEM_HPP

#include <functional>

enum class Trigger { DAM_MOD, DAM_REDUCE, ON_HIT, ON_KILL };

class Item {
public:
  Item(std::string id);
  union {
    std::function<int(int)> modify; // DAM_MOD, DAM_REDUCE
    std::function<void(void)> generic_effect; // ON_HIT, ON_KILL
  };
  Trigger trigger;
};

#endif //DEFINED_ITEM_HPP
