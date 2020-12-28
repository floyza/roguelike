#ifndef DEFINED_GUI_HPP
#define DEFINED_GUI_HPP

#include <memory>
#include <string>
#include <deque>
#include <libtcod/libtcod.hpp>
#include "bordered_window.hpp"

struct Message {
  Message(const std::string& text="", const TCODColor &color=TCODColor::white, bool center=false);
  std::string text;
  TCODColor color;
  bool center;
};

class Gui {
  Bordered_window con;
  void draw_msgs();
  std::deque<Message> lines;
  void add_line(const Message &msg);
public:
  Gui(int x, int y, int w, int h);
  void draw();
  Gui &send_msg(const Message &msg);
  Gui &send_nl();
};

class Input_gui : public Gui {
  public:
    using Callback_t = std::function<void(std::string_view)>;
  private:
    Callback_t callback;
  public:
    void recieve_input(std::string_view input);
    void set_input_handler(Callback_t func);
};

//Gui &operator<<(Gui &gui, const std::string &msg);

#endif
