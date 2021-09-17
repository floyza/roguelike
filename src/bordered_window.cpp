#include "bordered_window.hpp"

Bordered_window::Bordered_window(Pos pos, int width, int height)
    : border(width, height), window(width - 2, height - 2), border_pos(pos),
      window_pos(pos + Pos{1, 1}) {
  draw_border();
}

void Bordered_window::draw_border() {
  const int width = border.getWidth();
  const int height = border.getHeight();

  for (int x = 1; x < width - 1; ++x) {
    border.putChar(
        x, 0,
        wall_merge(TCOD_CHAR_HLINE,
                   TCODConsole::root->getChar(x + border_pos.x, border_pos.y)));
    border.putChar(x, height - 1,
                   wall_merge(TCOD_CHAR_HLINE, TCODConsole::root->getChar(
                                                   x + border_pos.x,
                                                   border_pos.y + height - 1)));
  }
  for (int y = 1; y < height - 1; ++y) {
    border.putChar(
        0, y,
        wall_merge(TCOD_CHAR_VLINE,
                   TCODConsole::root->getChar(border_pos.x, border_pos.y + y)));
    border.putChar(width - 1, y,
                   wall_merge(TCOD_CHAR_VLINE,
                              TCODConsole::root->getChar(
                                  border_pos.x + width - 1, border_pos.y + y)));
  }
  border.putChar(0, 0,
                 wall_merge(TCOD_CHAR_NW, TCODConsole::root->getChar(
                                              border_pos.x, border_pos.y)));
  border.putChar(width - 1, 0,
                 wall_merge(TCOD_CHAR_NE,
                            TCODConsole::root->getChar(border_pos.x + width - 1,
                                                       border_pos.y + 0)));
  border.putChar(
      0, height - 1,
      wall_merge(TCOD_CHAR_SW, TCODConsole::root->getChar(
                                   border_pos.x, border_pos.y + height - 1)));
  border.putChar(width - 1, height - 1,
                 wall_merge(TCOD_CHAR_SE, TCODConsole::root->getChar(
                                              border_pos.x + width - 1,
                                              border_pos.y + height - 1)));
}

TCODConsole *Bordered_window::operator->() { return &window; }

TCODConsole &Bordered_window::operator*() { return window; }

void Bordered_window::blit() {
  draw_border(); // maybe we don't need to do this every time?
  TCODConsole::blit(&window, 0, 0, 0, 0, &border, 1, 1);
  TCODConsole::blit(&border, 0, 0, 0, 0, TCODConsole::root, border_pos.x,
                    border_pos.y);
}
