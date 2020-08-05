#include "command.hpp"
#include "game.hpp"
#include "player.hpp"

int Move_command::execute() {
  if (target.do_move(target.pos+pos_)) {
    return 100;
  }
  return 0;
}
