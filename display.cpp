#include "display.hpp"
#include "tcod.hpp"
#include "drawable.hpp"

Display::Display(int w, int h, std::string title)
  : console{std::make_unique<TCODConsole>(w,h)}
{
  TCODConsole::initRoot(w,h,title.c_str());
}

bool Display::is_open() {
  return !TCODConsole::isWindowClosed();
}

void Display::draw(const Drawable &d) {
  for (int x=0; x<d.map.size(); ++x) {
    for (int y=0; y<d.map[x].size(); ++y) {
      TCODConsole::root->putChar(x+d.x, y+d.x, d.map[x][y]);
    }
  }
  //TCODConsole::root->printRect(d.x, d.y, d.map.size(), d.map[0].size(), d.map.data());
}

void Display::clear() {
  TCODConsole::root->clear();
}

void Display::flush() {
  TCODConsole::flush();
}
