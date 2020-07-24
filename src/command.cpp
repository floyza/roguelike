#include "command.hpp"
#include "game.hpp"
#include "player.hpp"

int Move_command::execute() {
  if (target.do_move(target.x+x_, target.y+y_)) {
    return 100;
  }
  return 0;
}
