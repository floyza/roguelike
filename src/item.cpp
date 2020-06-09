#include "item.hpp"
#include "lua.hpp"
#include "game.hpp"
#include "creature.hpp"

Item::Item(const std::string &id)
  : Item(id, 0, 0)
{
}

Item::Item(const std::string &id, int x, int y)
  : x(x), y(y)
{
  const Lua_item &item_base = game->get_item(id);
  trigger = item_base.type;
  name = item_base.name;

  if (is_trigger_type<modify_func>(trigger))
    effect = modify_func([&item_base](int &i) { i = (*game->lua_state)[item_base.func](i); });
  else if (is_trigger_type<generic_func>(trigger))
    effect = generic_func([&item_base] { (*game->lua_state)[item_base.func](); });
  else if (is_trigger_type<target_modify_func>(trigger))
    effect = target_modify_func([&item_base](int &i, Creature &c) { i = (*game->lua_state)[item_base.func](i, c); });
  else if (is_trigger_type<target_generic_func>(trigger))
    effect = target_generic_func([&item_base](Creature &c) { (*game->lua_state)[item_base.func](c); });
}

Item::~Item() = default;

void Item::draw() const {
  TCODConsole::root->setCharForeground(x,y,TCODColor(0xff,0xff,0xff));
  TCODConsole::root->setChar(x,y,'*');
}
