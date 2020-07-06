#ifndef GAME_HPP_DEFINED
#define GAME_HPP_DEFINED

#include "sol.hpp"
#include <memory>
#include <vector>

class Map;
class Display;
class Player;
class Gui;
class Lua_manager;
struct Message;

struct Lua_item;
struct Lua_monster;
struct Lua_status;

class Game {
  std::vector<Map> levels;
  static constexpr int log_width = 20;
  static constexpr int map_width = 100;
  static constexpr int map_height = 50;
  static const char *font_file;

  std::unique_ptr<Gui> log_header;
  std::unique_ptr<Gui> msg_log;
public:
  Game();
  Game(const Game &) = delete;
  Game& operator=(const Game &) = delete;
  ~Game();
  std::unique_ptr<Player> you;
  Map *map;

  void send_msg(const Message &msg);

  std::unique_ptr<Lua_manager> lua_manager;

  /** Returns true if game is not over
   */
  bool do_turn();
  void generate_map();
};

extern std::unique_ptr<Game> game;

#endif //GAME_HPP_DEFINED
