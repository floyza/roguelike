#ifndef __LUA_GUI_H_
#define __LUA_GUI_H_

#include "bordered_window.hpp"
#include "gui.hpp"
#include "pos.hpp"

class Lua_gui {
private:
  Gui messages;
  Bordered_window window;
  std::function<void()> close_callback;
  std::string current_entry;

public:
  Lua_gui(Pos pos, int width, int height, std::function<void()> close_callback);
  void draw();
  bool handle_input(const TCOD_key_t &input);
  void close();
};

#endif // __LUA_GUI_H_
