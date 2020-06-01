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
    you(std::make_unique<Player>('@', TCODColor::white)),
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
  if (!TCODConsole::isWindowClosed()) {
    TCODConsole::root->clear();
    map->draw();
    you->draw();
    msg_log->draw();
    log_header->draw();
    TCODConsole::root->flush();
    you->do_move();
    for (const std::unique_ptr<Monster> &mon : map->monsters)
      mon->do_move();
    return true;
  }
  return false;
}
