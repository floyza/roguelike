#include "lua.hpp"
#include "game.hpp"
#include "item.hpp"

Lua_item::Lua_item() = default;

Lua_item::Lua_item(Trigger type, std::string name)
  : type(type), name(name)
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
				{"ON_KILL", Trigger::ON_KILL}
			       });
  lua_state->new_usertype<Lua_item>("Item",
				    sol::constructors<Lua_item(),Lua_item(Trigger, std::string)>(),
				    "type", &Lua_item::type,
				    "name", &Lua_item::name);

  lua_state->script_file("items.lua");

  sol::table table = (*lua_state)["item_table"];
  for (const auto &[key, obj] : table) {
    items[key.as<std::string>()] = obj.as<Lua_item>();
  }
}
