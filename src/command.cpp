#include "command.hpp"
#include "game.hpp"
#include "player.hpp"
#include "map.hpp"

int Move_command::execute() {
  if (target.do_move(target.pos+pos_)) {
    return 100;
  }
  return 0;
}

int Stairs_move_command::execute() {
  switch (dir) {
    case Direction::Up:
      if (target.pos == game->map().entrance())
        if (game->move_upstairs())
          return 100;
      break;
    case Direction::Down:
      if (target.pos == game->map().exit())
        if (game->move_downstairs())
          return 100;
      break;
  }
  return 0;
}
