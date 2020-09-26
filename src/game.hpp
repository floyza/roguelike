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
  private:
    std::vector<Map> levels;
    int current_level;
    static const char *font_file;

    std::unique_ptr<Gui> log_header;
    std::unique_ptr<Gui> msg_log;
  public:
    Game();
    Game(const Game &) = delete;
    Game& operator=(const Game &) = delete;
    ~Game();
    std::unique_ptr<Player> you;

    void start();
    Map &map();
    const Map &map() const;
    int depth() { return current_level; }
    bool move_upstairs();
    bool move_downstairs();

    void send_msg(const Message &msg);

    std::unique_ptr<Lua_manager> lua_manager;

    void play();
    void generate_map();
    static constexpr int map_height = 50;
    static constexpr int map_width = 100;
    static constexpr int log_width = 20;
};

extern std::unique_ptr<Game> game;

#endif //GAME_HPP_DEFINED
