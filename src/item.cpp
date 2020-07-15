#include "item.hpp"
#include "effect.hpp"
#include "lua.hpp"
#include "game.hpp"

Item::Item(int id, int x, int y)
  : Item(game->lua_manager->get_item(id), x, y)
{
}

Item::Item(const std::string &name, int x, int y)
  : Item(game->lua_manager->get_item(name), x, y)
{
}

Item::Item(const Lua_item &base, int x, int y)
  : effect(base.type, base.func), id_(base.id), name_(base.name), x(x), y(y)
{
}

Item::~Item() = default;

void Item::draw() const {
  TCODConsole::root->setCharForeground(x,y,TCODColor(0xff,0xff,0xff));
  TCODConsole::root->setChar(x,y,'*');
}

Trigger Item::get_trigger() { return effect.get_trigger(); }

int Item::id() const { return id_; }

const std::string &Item::name() const { return name_; }
