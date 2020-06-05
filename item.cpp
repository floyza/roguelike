#include "item.hpp"
#include "lua.hpp"
#include "game.hpp"

Item::Item(const std::string &id)
{
  const Lua_item &item_base = g->get_item(id);
  trigger = item_base.type;
  name = item_base.name;
  switch (trigger) {
  case Trigger::DAM_MOD: case Trigger::DAM_REDUCE:
    effect = modify_func([&g, &item_base](int source) ->int { return (*g->lua_state)[item_base.func](source); });
    break;
  case Trigger::ON_HIT: case Trigger::ON_KILL: case Trigger::ON_MOVE:
    effect = generic_func([&g, &item_base] { (*g->lua_state)[item_base.func](); });
    break;
  default:
    throw std::runtime_error{"Item::Item: invalid trigger"};
  }
}

Item::~Item() = default;
