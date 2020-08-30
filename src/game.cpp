#include "game.hpp"
#include "lua.hpp"
#include "map.hpp"
#include "player.hpp"
#include "gui.hpp"
#include "monster.hpp"
#include "tcod_util.hpp"
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
  TCODConsole::initRoot(map_width + log_width, map_height, "Roguelike");
}

Game::~Game() = default;

void Game::generate_map() {
  game->map = new Map(map_width, map_height, 0);
}

bool Game::do_turn() {
  if (!TCODConsole::isWindowClosed() && !you->is_dead()) {
    TCODConsole::root->clear();
    map->draw();
    you->draw();
    msg_log->draw();
    log_header->draw();
    TCODConsole::root->flush();
    you->do_turn();
    for (Monster &mon : map->monsters)
      mon.do_turn();
    for (Monster &mon : map->monsters)
      mon.push_death();
    return true;
  }
  return false;
}

void Game::send_msg(const Message &msg) {
  msg_log->send_msg(msg);
  msg_log->send_nl();
}
