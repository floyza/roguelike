#include "item.hpp"
#include "effect.hpp"
#include "lua.hpp"
#include "game.hpp"

Item::Item(const std::string &id)
  : Item(id, 0, 0)
{
}

Item::Item(const std::string &id, int x, int y)
  : x(x), y(y)
{
  const Lua_item &item_base = game->get_item(id);
  name = item_base.name;
  effect = Effect{item_base.type, item_base.func};
}

Item::~Item() = default;

void Item::draw() const {
  TCODConsole::root->setCharForeground(x,y,TCODColor(0xff,0xff,0xff));
  TCODConsole::root->setChar(x,y,'*');
}

Trigger Item::get_trigger() { return effect.get_trigger(); }
