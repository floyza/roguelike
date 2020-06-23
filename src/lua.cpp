#include "lua.hpp"
#include "game.hpp"
#include "player.hpp"
#include "item.hpp"
#include "gui.hpp"
#include "map.hpp"
#include "monster.hpp"
#include "mon_id.hpp"
#include "tcod_util.hpp"

Lua_item::Lua_item() = default;

Lua_item::Lua_item(Trigger type, const std::string &func, const std::string &name, int rarity)
  : type(type), func(func), name(name), rarity(rarity)
{
}

void Game::init_lua() {
  lua_state = std::make_unique<sol::state>();
  lua_state->open_libraries(sol::lib::base, sol::lib::math);

  lua_state->new_enum<Trigger>("Trigger",
			       {
				{"DAM_MOD", Trigger::DAM_MOD},
				{"DAM_REDUCE", Trigger::DAM_REDUCE},
				{"ON_HIT", Trigger::ON_HIT},
				{"ON_ATTACK", Trigger::ON_ATTACK},
				{"ON_KILL", Trigger::ON_KILL},
				{"ON_MOVE", Trigger::ON_MOVE},
				{"ON_TURN", Trigger::ON_TURN},
				{"ON_DAM", Trigger::ON_DAM}
			       });
  lua_state->new_usertype<Lua_item>("Item",
				    sol::constructors<Lua_item(),Lua_item(Trigger, const std::string &, const std::string &, int)>(),
				    "type", &Lua_item::type,
				    "func", &Lua_item::func,
				    "name", &Lua_item::name,
				    "rarity", &Lua_item::rarity);
  lua_state->new_usertype<TCODColor>("color",
				     sol::constructors<TCODColor(),TCODColor(int,int,int)>(),
				     "r", &TCODColor::r,
				     "g", &TCODColor::g,
				     "b", &TCODColor::b);
  lua_state->new_usertype<mon_id>("mon_id",
				  sol::constructors<mon_id(), mon_id(char, const TCODColor &, const std::string &, int, int, int)>(),
				  "icon", &mon_id::icon,
				  "color", &mon_id::color,
				  "name", &mon_id::name,
				  "max_hp", &mon_id::max_hp,
				  "attack", &mon_id::attack,
				  "rarity", &mon_id::rarity);
  lua_state->new_usertype<Creature>("Creature",
				    sol::no_constructor,
				    "hp", sol::property(&Creature::get_hp),
				    "attack", sol::property(&Creature::get_attack),
				    "die", &Creature::die,
				    "x", &Creature::x,
				    "y", &Creature::y);
  lua_state->new_usertype<Player>("Player",
				  sol::constructors<Player(char, const TCODColor &, int, int, int, int)>(),
				  "hp", sol::property(&Creature::get_hp),
				  "attack", sol::property(&Creature::get_attack),
				  "do_attack", &Player::do_attack,
				  "do_attack_sans_triggers", &Player::do_attack_sans_triggers,
				  // make sure we get the correct overloaded take_damage function
				  "take_damage", static_cast<void(Player::*)(int)>(&Player::take_damage),
				  "turn_count", sol::property(&Player::turn_count),
				  "die", &Player::die,
				  "x", &Player::x,
				  "y", &Player::y);
  lua_state->new_usertype<Monster>("Monster",
				   sol::constructors<Monster(const std::string &, Map &, int, int)>(),
				   "hp", sol::property(&Creature::get_hp),
				   "attack", sol::property(&Creature::get_attack),
				   "name", sol::property(&Monster::name),
				   "do_attack", &Monster::do_attack,
				   "die", &Monster::die,
				   "x", &Monster::x,
				   "y", &Monster::y);
  lua_state->new_usertype<Map>("Map",
			       sol::constructors<Map(int, int, int)>(),
			       "monsters", &Map::monsters,
			       "items", &Map::items,
			       "width", sol::property(&Map::get_width),
			       "height", sol::property(&Map::get_height),
			       "is_walkable", &Map::is_walkable,
			       "set_walkable", &Map::set_walkable,
			       "in_fov", &Map::in_fov);
  lua_state->new_usertype<Game>("Game",
				sol::no_constructor,
				"you", sol::property([](Game &self){return std::ref(self.you);}),
				"send_msg", &Game::send_msg,
				"map", &Game::map);
  lua_state->new_usertype<Message>("Message",
				   sol::constructors<Message(), Message(const std::string &), Message(const std::string &, const TCODColor &), Message(const std::string &, const TCODColor &, bool)>());

  lua_state->set("game", std::ref(*game));
  lua_state->set("rand_int", rand_int);
  lua_state->set("percent_chance", percent_chance);

  lua_state->script_file("data/lua/items.lua");
  lua_state->script_file("data/lua/monsters.lua");

  sol::table item_table = (*lua_state)["item_table"];
  for (const auto &[key, obj] : item_table) {
    item_generators[key.as<std::string>()] = obj.as<Lua_item>();
  }
  sol::table mon_table = (*lua_state)["monster_table"];
  for (const auto &[key, obj] : mon_table) {
    monster_generators[key.as<std::string>()] = obj.as<mon_id>();
  }
}
