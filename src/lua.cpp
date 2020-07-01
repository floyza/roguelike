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

Lua_status::Lua_status() = default;

Lua_status::Lua_status(Trigger type, const std::string &func, const std::string &name, int duration)
  : type(type), func(func), name(name), duration(duration)
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
  lua_state->new_usertype<Lua_status>("Status",
				    sol::constructors<Lua_status(),Lua_status(Trigger, const std::string &, const std::string &, int)>(),
				    "type", &Lua_status::type,
				    "func", &Lua_status::func,
				    "name", &Lua_status::name,
				    "duration", &Lua_status::duration);
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
				    "do_move", &Creature::do_move,
				    "die", &Creature::die,
				    "x", &Creature::x,
				    "y", &Creature::y);
  lua_state->new_usertype<Player>("Player",
				  sol::constructors<Player(char, const TCODColor &, int, int, int, int)>(),
				  "hp", sol::property(&Creature::get_hp),
				  "attack", sol::property(&Creature::get_attack),
				  "do_attack", &Player::do_attack,
				  "do_attack_sans_triggers", &Player::do_attack_sans_triggers,
				  "do_move", &Player::do_move,
				  "aquire_item", static_cast<void(Player::*)(const std::string &)>(&Player::aquire_item),
				  "aquire_item", static_cast<void(Player::*)(const Lua_item &)>(&Player::aquire_item),
				  "aquire_status", static_cast<void(Player::*)(const std::string &)>(&Player::aquire_status),
				  "aquire_status", static_cast<void(Player::*)(const Lua_status &)>(&Player::aquire_status),
				  "remove_status", &Player::remove_status,
				  // make sure we get the correct overloaded take_damage function
				  "take_damage", static_cast<void(Player::*)(int)>(&Player::take_damage),
				  "turn_count", sol::property(&Player::turn_count),
				  "die", &Player::die,
				  "is_dead", &Player::is_dead,
				  "x", &Player::x,
				  "y", &Player::y);
  lua_state->new_usertype<Monster>("Monster",
				   sol::constructors<Monster(const std::string &, Map &, int, int), Monster(const mon_id &, Map &, int, int)>(),
				   "hp", sol::property(&Creature::get_hp),
				   "attack", sol::property(&Creature::get_attack),
				   "name", sol::property(&Monster::name),
				   "do_attack", &Monster::do_attack,
				   "do_move", &Monster::do_move,
				   "die", &Monster::die,
				   "x", &Monster::x,
				   "y", &Monster::y);
  lua_state->new_usertype<Tile>("Tile",
				sol::constructors<Tile(bool)>(),
				"discovered", &Tile::discovered);
  lua_state->new_usertype<Map>("Map",
			       sol::constructors<Map(int, int, int)>(),
			       "monsters", &Map::monsters,
			       "items", &Map::items,
			       "width", sol::property(&Map::get_width),
			       "height", sol::property(&Map::get_height),
			       "is_walkable", &Map::is_walkable,
			       "set_walkable", &Map::set_walkable,
			       "tile", &Map::tile,
			       "in_fov", &Map::in_fov);
  lua_state->new_usertype<Game>("Game",
				sol::no_constructor,
				"you", sol::property([](Game &self){return std::ref(self.you);}),
				"send_msg", &Game::send_msg,
				"map", &Game::map,
				"get_item", &Game::get_item,
				"get_mon", &Game::get_mon,
				"get_status", &Game::get_status,
				"get_rand_item", &Game::get_rand_item,
				"get_rand_mon", &Game::get_rand_mon);
  lua_state->new_usertype<Message>("Message",
				   sol::constructors<Message(), Message(const std::string &), Message(const std::string &, const TCODColor &), Message(const std::string &, const TCODColor &, bool)>());

  lua_state->set("game", std::ref(*game));
  lua_state->set("rand_int", rand_int);
  lua_state->set("percent_chance", percent_chance);

  lua_state->script_file("data/lua/items.lua");
  lua_state->script_file("data/lua/monsters.lua");
  lua_state->script_file("data/lua/status.lua");

  // sol::table::size only works for numerical keys

  sol::table item_table = (*lua_state)["item_table"];
  int table_size=0;
  for (auto i=item_table.begin(); i!=item_table.end(); ++i)
    ++table_size;

  item_generators.reserve(table_size);
  for (const auto &[key, obj] : item_table) {
    item_generators.push_back(obj.as<Lua_item>());
    Lua_item &o = item_generators.back();
    item_name_map.insert({key.as<std::string>(), &o});
    item_rarity_map.insert({o.rarity, &o});
  }

  sol::table mon_table = (*lua_state)["monster_table"];
  table_size=0;
  for (auto i=mon_table.begin(); i!=mon_table.end(); ++i)
    ++table_size;

  monster_generators.reserve(table_size);
  for (const auto &[key, obj] : mon_table) {
    monster_generators.push_back(obj.as<mon_id>());
    mon_id &o = monster_generators.back();
    monster_name_map.insert({key.as<std::string>(), &o});
    monster_rarity_map.insert({o.rarity, &o});
  }

  sol::table status_table = (*lua_state)["status_table"];
  table_size=0;
  for (auto i=status_table.begin(); i!=status_table.end(); ++i)
    ++table_size;

  status_generators.reserve(table_size);
  for (const auto &[key, obj] : status_table) {
    status_generators.push_back(obj.as<Lua_status>());
    Lua_status &o = status_generators.back();
    status_name_map.insert({key.as<std::string>(), &o});
  }
}
