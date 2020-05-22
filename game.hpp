#ifndef GAME_HPP_DEFINED
#define GAME_HPP_DEFINED

#include <memory>
#include <vector>

class Map;
class Display;
class Player;

class Game {
  std::vector<Map> levels;
public:
  std::unique_ptr<Player> you;
  Map *map;

  /**
   * Returns true if game is not over
   */
  bool do_turn();
};

extern std::unique_ptr<Game> g;

#endif //GAME_HPP_DEFINED
