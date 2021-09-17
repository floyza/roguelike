#ifndef __INVENTORY_GUI_H_
#define __INVENTORY_GUI_H_

#include "bordered_window.hpp"
#include "pos.hpp"

class Player;

class Inventory_gui {
private:
  Bordered_window window;
  Player &target;
  int index = 0;
  void update_window();
  std::function<void()> close_callback;

public:
  Inventory_gui(Pos pos, int width, int height, Player &player,
                std::function<void()> close_callback);
  void draw();
  bool handle_input(const TCOD_key_t &input);
  void close();
};

#endif // __INVENTORY_GUI_H_
