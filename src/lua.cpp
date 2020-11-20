#include "lua.hpp"
#include "game.hpp"
#include "player.hpp"
#include "item.hpp"
#include "gui.hpp"
#include "map.hpp"
#include "monster.hpp"
#include "tcod_util.hpp"
#include "pos.hpp"
#include <iostream>

Lua_manager::Lua_manager()
{
}

void Lua_manager::init() {
  lua_state.open_libraries(sol::lib::base, sol::lib::math, sol::lib::string);

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
  lua_state.new_usertype<TCODColor>("color",
																		sol::constructors<TCODColor(),TCODColor(int,int,int)>(),
																		"r", &TCODColor::r,
																		"g", &TCODColor::g,
																		"b", &TCODColor::b);
  lua_state.new_usertype<Pos>("Pos",
															sol::constructors<Pos(), Pos(int,int)>(),
															"x", &Pos::x,
															"y", &Pos::y);
  lua_state.new_usertype<Creature>("Creature",
																	 sol::no_constructor,
																	 "hp", sol::property(&Creature::get_hp),
																	 "attack", sol::property(&Creature::get_attack),
																	 "do_turn", &Creature::do_turn,
																	 "die", &Creature::die,
																	 "pos", &Creature::pos);
  lua_state.new_usertype<Player>("Player",
																 sol::constructors<Player(char, const TCODColor &, int, int, const Pos &)>(),
																 "hp", sol::property(&Creature::get_hp),
																 "attack", sol::property(&Creature::get_attack),
																 // "do_attack", sol::overload(&Player::do_attack,
																 // 														[](Player &self, Creature &target){
																 // 															self.do_attack(target);}),
																 "do_attack", [](Player &self, Creature &target, sol::optional<bool> triggers) {
																	 if (triggers.has_value()) {
																		 self.do_attack(target, *triggers);
																	 } else {
																		 self.do_attack(target);
																	 }
																 },
																 "do_turn", &Player::do_turn,
																 "do_move", &Player::do_move,
																 "aquire_item", sol::overload(
																															static_cast<void(Player::*)(int)>(&Player::aquire_item),
																															static_cast<void(Player::*)(const std::string &)>(&Player::aquire_item),
																															static_cast<void(Player::*)(const Lua_item &)>(&Player::aquire_item),
																															static_cast<void(Player::*)(const Item &)>(&Player::aquire_item)),
																 "aquire_status", sol::overload(
																																static_cast<void(Player::*)(int)>(&Player::aquire_status),
																																static_cast<void(Player::*)(const std::string &)>(&Player::aquire_status),
																																static_cast<void(Player::*)(const Lua_status &)>(&Player::aquire_status)),
																 "remove_status", static_cast<void(Player::*)(int)>(&Player::remove_status),
																 "remove_status", static_cast<void(Player::*)(const std::string &)>(&Player::remove_status),
																 // make sure we get the correct overloaded take_damage function
																 "take_damage", static_cast<void(Player::*)(int)>(&Player::take_damage),
																 "turn_count", sol::property(&Player::turn_count),
																 "die", &Player::die,
																 "is_dead", &Player::is_dead,
																 "pos", &Player::pos);
  lua_state.new_usertype<Monster>("Monster",
																	sol::constructors<Monster(int, int, const Pos &), Monster(const std::string &, int, const Pos &), Monster(const Lua_monster &, int, const Pos &)>(),
																	"hp", sol::property(&Creature::get_hp),
																	"attack", sol::property(&Creature::get_attack),
																	"name", sol::property(&Monster::name),
																	"do_attack", &Monster::do_attack,
																	"do_turn", &Monster::do_turn,
																	"die", &Monster::die,
																	"pos", &Monster::pos);
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
															 "you", sol::property([](Game &self){
    return std::ref(self.you);}),
															 "lua_manager", sol::property([](Game &self){
    return std::ref(self.lua_manager);}),
															 "send_msg", &Game::send_msg,
															 "map", sol::property([](Game &self){
                                 return std::ref(self.map());
                               }));
  lua_state.new_usertype<Lua_manager>("Lua_manager",
																			sol::no_constructor,
																			"get_func", &Lua_manager::get_func,
																			"get_item", static_cast<const Lua_item &(Lua_manager::*)(int) const>(&Lua_manager::get_item),
																			"get_item", static_cast<const Lua_item &(Lua_manager::*)(const std::string &) const>(&Lua_manager::get_item),
																			"get_mon", static_cast<const Lua_monster &(Lua_manager::*)(int) const>(&Lua_manager::get_mon),
																			"get_mon", static_cast<const Lua_monster &(Lua_manager::*)(const std::string &) const>(&Lua_manager::get_mon),
																			"get_status", static_cast<const Lua_status &(Lua_manager::*)(int) const>(&Lua_manager::get_status),
																			"get_status", static_cast<const Lua_status &(Lua_manager::*)(const std::string &) const>(&Lua_manager::get_status),
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

  sol::table item_table = lua_state[item_def_table];
  item_generators.reserve(item_table.size());
  item_table.for_each([this](sol::object i, sol::object) {
		item_generators.push_back(load_item(i.as<int>()));
		Lua_item &o = item_generators.back();
		item_rarity_map.insert({o.rarity, &o});
	});

  sol::table mon_table = lua_state[monster_def_table];
  monster_generators.reserve(mon_table.size());
  mon_table.for_each([this](sol::object i, sol::object) {
		monster_generators.push_back(load_monster(i.as<int>()));
		Lua_monster &o = monster_generators.back();
		monster_rarity_map.insert({o.rarity, &o});
	});
  
  sol::table status_table = lua_state[status_def_table];
  status_generators.reserve(status_table.size());
  status_table.for_each([this](sol::object i, sol::object) {
		status_generators.push_back(load_status(i.as<int>()));
		// Lua_status &o = status_generators.back();
	});
}

Lua_item Lua_manager::load_item(int id) {
  Lua_item item;
  item.id = id-1;
  mandatory(item_def_table, id, "name", item.name);
  mandatory(item_def_table, id, "trigger", item.type);
  mandatory(item_def_table, id, "effect", item.func);
  mandatory(item_def_table, id, "rarity", item.rarity);
  return item;
}

Lua_status Lua_manager::load_status(int id) {
  Lua_status status;
  status.id = id-1;
  mandatory(status_def_table, id, "name", status.name);
  mandatory(status_def_table, id, "trigger", status.type);
  mandatory(status_def_table, id, "effect", status.func);
  mandatory(status_def_table, id, "duration", status.duration);
  return status;
}

Lua_monster Lua_manager::load_monster(int id) {
  Lua_monster monster;
  monster.id = id-1;
  mandatory(monster_def_table, id, "name", monster.name);
  mandatory(monster_def_table, id, "icon", monster.icon);
  mandatory(monster_def_table, id, "color", monster.color);
  mandatory(monster_def_table, id, "hp", monster.max_hp);
  mandatory(monster_def_table, id, "attack", monster.attack);
  mandatory(monster_def_table, id, "rarity", monster.rarity);
  return monster;
}

void Lua_manager::load_file(const std::string &file) {
  lua_state.script_file(file);
}

void Lua_manager::script(const std::string &input) {
  lua_state.script(input);
}

void Lua_manager::script_cin() {
  std::string in;
  while (std::getline(std::cin, in)) {
    script(in);
  }
}

sol::protected_function Lua_manager::get_func(const std::string &func) {
  return lua_state[func];
}

const Lua_item &Lua_manager::get_item(int id) const {
  return item_generators.at(id);
}

const Lua_item &Lua_manager::get_item(const std::string &name) const {
  return *std::find_if(item_generators.begin(), item_generators.end(),
											 [&name](const Lua_item &item){
												 return item.name == name;
											 });
}

const Lua_monster &Lua_manager::get_mon(int id) const {
  return monster_generators.at(id);
}

const Lua_monster &Lua_manager::get_mon(const std::string &name) const {
  return *std::find_if(monster_generators.begin(), monster_generators.end(),
											 [&name](const Lua_monster &monster){
												 return monster.name == name;
											 });
}

const Lua_status &Lua_manager::get_status(int id) const {
  return status_generators.at(id);
}

const Lua_status &Lua_manager::get_status(const std::string &name) const {
  return *std::find_if(status_generators.begin(), status_generators.end(),
											 [&name](const Lua_status &status){
												 return status.name == name;
											 });
}
const Lua_item &Lua_manager::get_rand_item(int depth) const {
  auto range = item_rarity_map.equal_range(rand_int_log_bounded(depth-2, depth+2, 0, game->max_depth));
  return *random_element(range.first, range.second)->second;
}

const Lua_monster &Lua_manager::get_rand_mon(int depth) const {
  auto range = monster_rarity_map.equal_range(rand_int_log_bounded(depth-2, depth+2, 0, game->max_depth));
  return *random_element(range.first, range.second)->second;
}
