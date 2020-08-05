#include "item.hpp"
#include "effect.hpp"
#include "lua.hpp"
#include "game.hpp"

Item::Item(int id, const Pos &pos)
  : Item(game->lua_manager->get_item(id), pos)
{
}

Item::Item(const std::string &name, const Pos &pos)
  : Item(game->lua_manager->get_item(name), pos)
{
}

Item::Item(const Lua_item &base, const Pos &pos)
  : effect(base.type, base.func), id_(base.id), name_(base.name), pos(pos)
{
}

Item::~Item() = default;

void Item::draw() const {
  TCODConsole::root->setCharForeground(pos.x,pos.y,TCODColor(0xff,0xff,0xff));
  TCODConsole::root->setChar(pos.x,pos.y,'*');
}

Trigger Item::get_trigger() { return effect.get_trigger(); }

int Item::id() const { return id_; }

const std::string &Item::name() const { return name_; }
