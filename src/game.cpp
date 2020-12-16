#include "game.hpp"
#include "lua.hpp"
#include "map.hpp"
#include "player.hpp"
#include "gui.hpp"
#include "monster.hpp"
#include "tcod_util.hpp"
#include "input_handler.hpp"
#include <iostream>
#include <libtcod/libtcod.hpp>
#include <lua.h>
#include <memory>

std::unique_ptr<Game> game;

const char *Game::font_file = "data/fonts/cp866_8x12.png";

Game::Game()
  : log_header(std::make_unique<Gui>(map_width, 0, log_width, 3)),
    msg_log(std::make_unique<Gui>(map_width, 2, log_width, map_height-2)),
    you(std::make_unique<Player>('@', TCODColor::white, 30, 5)),
    lua_manager(std::make_unique<Lua_manager>())
{
  log_header->send_msg({"LOG", TCODColor::white, true});
  TCODConsole::setCustomFont(font_file, TCOD_FONT_LAYOUT_ASCII_INROW);
  you->push_input_handler(std::make_unique<Player_input_handler>(*you));
}

Game::~Game() = default;

void Game::start() {
  TCODConsole::initRoot(map_width + log_width, map_height, "Roguelike");
  lua_manager->init();
  levels.push_back(Map{map_width, map_height, 0});
  current_level = 0;
  you->pos = map().entrance();
}

void Game::play() {
  while (!TCODConsole::isWindowClosed() && !you->is_dead()) {
    TCODConsole::root->clear();
    map().draw();
    you->draw();
    msg_log->draw();
    log_header->draw();
    map().do_turn();
    TCODConsole::root->flush();
  }
}

bool Game::move_upstairs() {
  if (current_level == 0)
    return false;
  --current_level;
  you->pos = map().entrance();
  return true;
}

bool Game::move_downstairs() {
  if (current_level == levels.size()-1) {
    levels.push_back(Map{map_width, map_height, current_level+1});
  }
  ++current_level;
  you->pos = map().entrance();
  return true;
}

void Game::send_msg(const Message &msg) {
  msg_log->send_msg(msg);
  msg_log->send_nl();
}

Map &Game::map() {
  return levels[current_level];
}
const Map &Game::map() const {
  return levels[current_level];
}
