#ifndef __BORDERED_WINDOW_H_
#define __BORDERED_WINDOW_H_

#include "pos.hpp"
#include "tcod_util.hpp"

class Bordered_window {
private:
  TCODConsole border;
  TCODConsole window;
  Pos border_pos;
  Pos window_pos;
  void draw_border();

public:
  Bordered_window(Pos pos, int width, int height);
  TCODConsole *operator->();
  TCODConsole &operator*();
  void blit();
};

#endif // __BORDERED_WINDOW_H_
