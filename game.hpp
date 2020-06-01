#ifndef GAME_HPP_DEFINED
#define GAME_HPP_DEFINED

#include <memory>
#include <vector>

class Map;
class Display;
class Player;
class Gui;

class Game {
  std::vector<Map> levels;
  static constexpr int log_width = 20;
  static constexpr int map_width = 100;
  static constexpr int map_height = 50;
  static const char *font_file;

  std::unique_ptr<Gui> log_header;
public:
  Game();
  ~Game();
  std::unique_ptr<Player> you;
  std::unique_ptr<Gui> msg_log;
  Map *map;

  /**
   * Returns true if game is not over
   */
  bool do_turn();
  void generate_map();
};

extern std::unique_ptr<Game> g;

#endif //GAME_HPP_DEFINED
