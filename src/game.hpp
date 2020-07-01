#ifndef GAME_HPP_DEFINED
#define GAME_HPP_DEFINED

#include "sol.hpp"
#include <memory>
#include <map>
#include <vector>
#include "lua.hpp"
#include "mon_id.hpp"

class Map;
class Display;
class Player;
class Gui;
struct Message;

class Game {
  std::vector<Map> levels;
  static constexpr int log_width = 20;
  static constexpr int map_width = 100;
  static constexpr int map_height = 50;
  static const char *font_file;

  std::unique_ptr<Gui> log_header;
  std::unique_ptr<Gui> msg_log;

  std::vector<Lua_item> item_generators;
  std::vector<mon_id> monster_generators;
  std::vector<Lua_status> status_generators;

  std::map<std::string, Lua_item *> item_name_map;
  std::map<std::string, mon_id *> monster_name_map;
  std::map<std::string, Lua_status *> status_name_map;

  std::multimap<int, Lua_item *> item_rarity_map;
  std::multimap<int, mon_id *> monster_rarity_map;
public:
  Game();
  Game(const Game &) = delete;
  Game& operator=(const Game &) = delete;
  ~Game();
  std::unique_ptr<Player> you;
  std::unique_ptr<sol::state> lua_state;
  Map *map;

  void send_msg(const Message &msg);

  /** Returns true if game is not over
   */
  bool do_turn();
  void generate_map();

  void init_lua();

  const Lua_item &get_item(const std::string &id) const;
  const mon_id &get_mon(const std::string &id) const;
  const Lua_status &get_status(const std::string &id) const;

  const Lua_item &get_rand_item(int depth) const;
  const mon_id &get_rand_mon(int depth) const;
};

extern std::unique_ptr<Game> game;

#endif //GAME_HPP_DEFINED
