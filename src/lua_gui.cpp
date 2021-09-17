#include "lua_gui.hpp"
#include "game.hpp"
#include "lua.hpp"

Lua_gui::Lua_gui(Pos pos, int width, int height,
                 std::function<void()> close_callback)
    : messages(pos.x, pos.y, width, height - 2),
      window(Pos{pos.x, pos.y + height - 3}, width, 3),
      close_callback(close_callback) {}

bool Lua_gui::handle_input(const TCOD_key_t &input) {
  if (input.vk == TCODK_ENTER) {
    game->lua_manager->script(current_entry);
    messages.send_msg(current_entry);
    current_entry = "";
  } else if (input.vk == TCODK_TEXT) {
    current_entry +=
        input.text; // doesn't seem to work? never get TCODK_TEXT event
  } else if (input.vk == TCODK_BACKSPACE) {
    if (!current_entry.empty())
      current_entry.pop_back();
  }
  return true;
}

void Lua_gui::draw() {
  window->clear();
  window->print(0, 0, current_entry);
  window.blit();
  messages.draw();
}

void Lua_gui::close() { close_callback(); }
