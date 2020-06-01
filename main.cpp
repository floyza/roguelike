#include "game.hpp"
#include "map.hpp"
#include "tcod_util.hpp"
#include "player.hpp"
#include "gui.hpp"
#include <iostream>

int main() {
  tcod_rand = std::make_unique<TCODRandom>();
  
  g = std::make_unique<Game>();
  g->generate_map();
  g->msg_log->send_msg({"You descend down the stairs."});
  g->msg_log->send_nl();
  g->msg_log->send_msg({"You hit the goblin for 13 damage."});
  g->msg_log->send_nl();
  g->msg_log->send_msg({"The goblin hits you for 4 damage."});
  g->msg_log->send_nl();
  g->msg_log->send_msg({"The goblin misses you."});
  
  while (g->do_turn()) ;
}
