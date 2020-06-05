#include "game.hpp"
#include "map.hpp"
#include "player.hpp"
#include "gui.hpp"
#include "monster.hpp"
#include <libtcod/libtcod.hpp>

std::unique_ptr<Game> g;

const char *Game::font_file = "cp866_8x12.png";

Game::Game()
  : log_header(std::make_unique<Gui>(map_width, 0, log_width, 3)),
    you(std::make_unique<Player>('@', TCODColor::white, 30, 5)),
    msg_log(std::make_unique<Gui>(map_width, 2, log_width, map_height-2))
{
  log_header->send_msg({"LOG", TCODColor::white, true});
  TCODConsole::setCustomFont(font_file, TCOD_FONT_LAYOUT_ASCII_INROW);
  TCODConsole::initRoot(map_width + log_width, map_height, "Roguelike");
}

Game::~Game() = default;

void Game::generate_map() {
  g->map = new Map(map_width, map_height, 0);
}

bool Game::do_turn() {
  if (!TCODConsole::isWindowClosed() || you->is_dead()) {
    TCODConsole::root->clear();
    map->draw();
    you->draw();
    msg_log->draw();
    log_header->draw();
    TCODConsole::root->flush();
    you->do_move();
    for (Monster &mon : map->monsters)
      mon.do_move();
    return true;
  }
  return false;
}

const Lua_item &Game::get_item(const std::string &id) const {
  auto iter = items.find(id);
  if (iter == items.end())
    throw std::runtime_error{"Game::get_item: invalid item id"};
  return iter->second;
}

const mon_id &Game::get_mon(const std::string &id) const {
  auto iter = monsters.find(id);
  if (iter == monsters.end())
    throw std::runtime_error{"Game::get_mon: invalid item id"};
  return iter->second;
}
