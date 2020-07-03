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

std::unique_ptr<Game> game;

const char *Game::font_file = "data/fonts/cp866_8x12.png";

Game::Game()
  : log_header(std::make_unique<Gui>(map_width, 0, log_width, 3)),
    msg_log(std::make_unique<Gui>(map_width, 2, log_width, map_height-2)),
    you(std::make_unique<Player>('@', TCODColor::white, 30, 5))
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
    you->do_move();
    for (Monster &mon : map->monsters)
      mon.do_move();
    return true;
  }
  return false;
}

const Lua_item &Game::get_item(const std::string &id) const {
  auto iter = item_name_map.find(id);
  if (iter == item_name_map.end())
    throw std::runtime_error{"Game::get_item: invalid item id"};
  return *iter->second;
}

const Lua_monster &Game::get_mon(const std::string &id) const {
  auto iter = monster_name_map.find(id);
  if (iter == monster_name_map.end())
    throw std::runtime_error{"Game::get_mon: invalid monster id"};
  return *iter->second;
}

const Lua_status &Game::get_status(const std::string &id) const {
  auto iter = status_name_map.find(id);
  if (iter == status_name_map.end())
    throw std::runtime_error{"Game::get_status: invalid status id"};
  return *iter->second;
}

const Lua_item &Game::get_rand_item(int depth) const {
  auto range = item_rarity_map.equal_range(depth);
  return *random_element(range.first, range.second)->second;
}

const Lua_monster &Game::get_rand_mon(int depth) const {
  auto range = monster_rarity_map.equal_range(depth);
  return *random_element(range.first, range.second)->second;
}

void Game::send_msg(const Message &msg) {
  msg_log->send_msg(msg);
  msg_log->send_nl();
}
