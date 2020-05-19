#ifndef DISPLAY_HPP_DEFINED
#define DISPLAY_HPP_DEFINED

#include <string>
#include <memory>

class TCODConsole;
struct Drawable;

class Display {
  std::unique_ptr<TCODConsole> console;
public:
  Display(int w, int h, std::string title);
  bool is_open();
  void draw(const Drawable &d);
  void clear();
  void flush();
};

#endif //DISPLAY_HPP_DEFINED
