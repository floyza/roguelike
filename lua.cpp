#include "lua.hpp"
#include "game.hpp"
#include "item.hpp"
#include "mon_id.hpp"

Lua_item::Lua_item() = default;

Lua_item::Lua_item(Trigger type, const std::string &func, const std::string &name)
  : type(type), func(func), name(name)
{
}

void Game::init_lua() {
  lua_state = std::make_unique<sol::state>();
  lua_state->open_libraries(sol::lib::base);

  lua_state->new_enum<Trigger>("Trigger",
			       {
				{"DAM_MOD", Trigger::DAM_MOD},
				{"DAM_REDUCE", Trigger::DAM_REDUCE},
				{"ON_HIT", Trigger::ON_HIT},
				{"ON_KILL", Trigger::ON_KILL},
				{"ON_MOVE", Trigger::ON_MOVE}
			       });
  lua_state->new_usertype<Lua_item>("Item",
				    sol::constructors<Lua_item(),Lua_item(Trigger, const std::string &, const std::string &)>(),
				    "type", &Lua_item::type,
				    "func", &Lua_item::func,
				    "name", &Lua_item::name);
  lua_state->new_usertype<TCODColor>("color",
				     sol::constructors<TCODColor(),TCODColor(int,int,int)>());
  lua_state->new_usertype<mon_id>("Monster",
				  sol::constructors<mon_id(), mon_id(char, const TCODColor &, const std::string &, int, int)>(),
				  "icon", &mon_id::icon,
				  "color", &mon_id::color,
				  "name", &mon_id::name,
				  "max_hp", &mon_id::max_hp,
				  "attack", &mon_id::attack);

  lua_state->script_file("items.lua");
  lua_state->script_file("monsters.lua");

  sol::table item_table = (*lua_state)["item_table"];
  for (const auto &[key, obj] : item_table) {
    items[key.as<std::string>()] = obj.as<Lua_item>();
  }
  sol::table mon_table = (*lua_state)["monster_table"];
  for (const auto &[key, obj] : mon_table) {
    monsters[key.as<std::string>()] = obj.as<mon_id>();
  }
}
