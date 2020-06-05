#include "item.hpp"
#include "lua.hpp"
#include "game.hpp"

Item::Item(const std::string &id)
{
  const Lua_item &item_base = g->get_item(id);
  trigger = item_base.type;
  name = item_base.name;

  if (has_arg(trigger))
    effect = modify_func([&g, &item_base](int source) ->int { return (*g->lua_state)[item_base.func](source); });
  else
    effect = generic_func([&g, &item_base] { (*g->lua_state)[item_base.func](); });
}

Item::~Item() = default;

bool has_arg(Trigger trigger) {
  if (trigger == Trigger::ON_HIT || trigger == Trigger::ON_KILL || trigger == Trigger::ON_MOVE) {
    return false;
  } else if (trigger == Trigger::DAM_MOD || trigger == Trigger::DAM_REDUCE) {
    return true;
  }
  throw std::runtime_error{"has_arg(Trigger): invalid trigger"};
}
