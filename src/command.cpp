#include "command.hpp"
#include "game.hpp"
#include "player.hpp"
#include "map.hpp"
#include "inventory_gui.hpp"
#include "lua_gui.hpp"

int Move_command::execute() {
  if (target.do_move(target.pos+pos_)) {
    return 100;
  }
  return 0;
}

int Stairs_move_command::execute() {
  switch (dir) {
    case Direction::Up:
      if (target.pos == game->map().entrance()) {
        if (game->move_upstairs()) {
          game->send_msg(std::string("You move upstairs."));
          return 100;
        }
      }
      break;
    case Direction::Down:
      if (target.pos == game->map().exit()) {
        if (game->move_downstairs()) {
          game->send_msg(std::string("You move upstairs."));
          return 100;
        }
        break;
      }
  }
  return 0;
}

Inventory_command::Inventory_command(Inventory_gui &gui, TCOD_key_t key)
  : gui{gui}, key{key}
{
}

int Inventory_command::execute() {
  gui.handle_input(key);
  return 0; // takes no time
}

Lua_command::Lua_command(Lua_gui &gui, TCOD_key_t key)
  : gui{gui}, key{key}
{
}

int Lua_command::execute() {
  if (gui.handle_input(key))
    gui.draw();
  return 0;
}

int Pickup_command::execute() {
  Map &map = game->map();
  for (auto it = map.items.begin(); it != map.items.end(); ++it) {
    Item &item = *it;
    if (item.pos == target_.pos) {
      using namespace std::string_literals;
      const std::string &name = item.name();
      game->send_msg("You pick up "s + (is_vowel(name[0]) ? "an " : "a ") + name + ".");
      target_.aquire_item(item);
      map.items.erase(it); // invalidates `item` reference
                           // and iterators
      break;
    }
  }
  return 100;
}
