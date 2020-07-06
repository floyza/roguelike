#include "lua.hpp"
#include "game.hpp"
#include "player.hpp"
#include "item.hpp"
#include "gui.hpp"
#include "map.hpp"
#include "monster.hpp"
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

Lua_monster::Lua_monster() = default;

Lua_monster::Lua_monster(char icon, const TCODColor &color, const std::string &name, int max_hp, int attack, int rarity)
  : icon(icon), color(color), name(name), max_hp(max_hp), attack(attack), rarity(rarity)
{
}

Lua_manager::Lua_manager()
{
  lua_state.open_libraries(sol::lib::base, sol::lib::math);

  lua_state.new_enum<Trigger>("Trigger",
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
  lua_state.new_usertype<Lua_item>("Lua_item",
				    sol::constructors<Lua_item(),Lua_item(Trigger, const std::string &, const std::string &, int)>(),
				    "type", &Lua_item::type,
				    "func", &Lua_item::func,
				    "name", &Lua_item::name,
				    "rarity", &Lua_item::rarity);
  lua_state.new_usertype<Lua_status>("Lua_status",
				    sol::constructors<Lua_status(),Lua_status(Trigger, const std::string &, const std::string &, int)>(),
				    "type", &Lua_status::type,
				    "func", &Lua_status::func,
				    "name", &Lua_status::name,
				    "duration", &Lua_status::duration);
  lua_state.new_usertype<TCODColor>("color",
				     sol::constructors<TCODColor(),TCODColor(int,int,int)>(),
				     "r", &TCODColor::r,
				     "g", &TCODColor::g,
				     "b", &TCODColor::b);
  lua_state.new_usertype<Lua_monster>("Lua_monster",
				       sol::constructors<Lua_monster(), Lua_monster(char, const TCODColor &, const std::string &, int, int, int)>(),
				       "icon", &Lua_monster::icon,
				       "color", &Lua_monster::color,
				       "name", &Lua_monster::name,
				       "max_hp", &Lua_monster::max_hp,
				       "attack", &Lua_monster::attack,
				       "rarity", &Lua_monster::rarity);
  lua_state.new_usertype<Creature>("Creature",
				    sol::no_constructor,
				    "hp", sol::property(&Creature::get_hp),
				    "attack", sol::property(&Creature::get_attack),
				    "do_move", &Creature::do_move,
				    "die", &Creature::die,
				    "x", &Creature::x,
				    "y", &Creature::y);
  lua_state.new_usertype<Player>("Player",
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
  lua_state.new_usertype<Monster>("Monster",
				   sol::constructors<Monster(const std::string &, Map &, int, int), Monster(const Lua_monster &, Map &, int, int)>(),
				   "hp", sol::property(&Creature::get_hp),
				   "attack", sol::property(&Creature::get_attack),
				   "name", sol::property(&Monster::name),
				   "do_attack", &Monster::do_attack,
				   "do_move", &Monster::do_move,
				   "die", &Monster::die,
				   "x", &Monster::x,
				   "y", &Monster::y);
  lua_state.new_usertype<Tile>("Tile",
				sol::constructors<Tile(bool)>(),
				"discovered", &Tile::discovered);
  lua_state.new_usertype<Map>("Map",
			       sol::constructors<Map(int, int, int)>(),
			       "monsters", &Map::monsters,
			       "items", &Map::items,
			       "width", sol::property(&Map::get_width),
			       "height", sol::property(&Map::get_height),
			       "is_walkable", &Map::is_walkable,
			       "set_walkable", &Map::set_walkable,
			       "tile", &Map::tile,
			       "in_fov", &Map::in_fov);
  lua_state.new_usertype<Game>("Game",
			       sol::no_constructor,
			       "you", sol::property([](Game &self){return std::ref(self.you);}),
			       "lua_manager", sol::property([](Game &self){return std::ref(self.lua_manager);}),
			       "send_msg", &Game::send_msg,
			       "map", &Game::map);
  lua_state.new_usertype<Lua_manager>("Lua_manager",
				      sol::constructors<Lua_manager>(),
				      "get_func", &Lua_manager::get_func,
				      "get_item", &Lua_manager::get_item,
				      "get_mon", &Lua_manager::get_mon,
				      "get_status", &Lua_manager::get_status,
				      "get_rand_item", &Lua_manager::get_rand_item,
				      "get_rand_mon", &Lua_manager::get_rand_mon);
  lua_state.new_usertype<Message>("Message",
				   sol::constructors<Message(), Message(const std::string &), Message(const std::string &, const TCODColor &), Message(const std::string &, const TCODColor &, bool)>());

  lua_state.set("game", std::ref(*game));
  lua_state.set("rand_int", rand_int);
  lua_state.set("percent_chance", percent_chance);

  load_file("data/lua/items.lua");
  load_file("data/lua/monsters.lua");
  load_file("data/lua/status.lua");

  // sol::table::size only works for numerical keys

  sol::table item_table = lua_state["item_table"];
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

  sol::table mon_table = lua_state["monster_table"];
  table_size=0;
  for (auto i=mon_table.begin(); i!=mon_table.end(); ++i)
    ++table_size;

  monster_generators.reserve(table_size);
  for (const auto &[key, obj] : mon_table) {
    monster_generators.push_back(obj.as<Lua_monster>());
    Lua_monster &o = monster_generators.back();
    monster_name_map.insert({key.as<std::string>(), &o});
    monster_rarity_map.insert({o.rarity, &o});
  }

  sol::table status_table = lua_state["status_table"];
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

void Lua_manager::load_file(const std::string &file) {
  lua_state.script_file("data/lua/items.lua");
}

sol::function Lua_manager::get_func(const std::string &func) {
  return lua_state[func];
}

const Lua_item &Lua_manager::get_item(const std::string &id) const {
  auto iter = item_name_map.find(id);
  if (iter == item_name_map.end())
    throw std::runtime_error{"Lua_manager::get_item: invalid item id"};
  return *iter->second;
}

const Lua_monster &Lua_manager::get_mon(const std::string &id) const {
  auto iter = monster_name_map.find(id);
  if (iter == monster_name_map.end())
    throw std::runtime_error{"Lua_manager::get_mon: invalid monster id"};
  return *iter->second;
}

const Lua_status &Lua_manager::get_status(const std::string &id) const {
  auto iter = status_name_map.find(id);
  if (iter == status_name_map.end())
    throw std::runtime_error{"Lua_manager::get_status: invalid status id"};
  return *iter->second;
}

const Lua_item &Lua_manager::get_rand_item(int depth) const {
  auto range = item_rarity_map.equal_range(depth);
  return *random_element(range.first, range.second)->second;
}

const Lua_monster &Lua_manager::get_rand_mon(int depth) const {
  auto range = monster_rarity_map.equal_range(depth);
  return *random_element(range.first, range.second)->second;
}
