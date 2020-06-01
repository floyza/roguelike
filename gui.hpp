#include <memory>
#include <string>
#include <deque>
#include <libtcod/libtcod.hpp>

class TCODConsole;

struct Message {
  Message(const std::string& text, const TCODColor &color=TCODColor::white, bool center=false);
  std::string text;
  TCODColor color;
  bool center;
};

class Gui {
  std::unique_ptr<TCODConsole> con;
  int rootx,rooty;
  void draw_border();
  void draw_msgs();
  std::deque<Message> lines;
  void add_line(const Message &msg);
public:
  Gui(int x, int y, int w, int h);
  void draw();
  Gui &send_msg(const Message &msg);
  Gui &send_nl();
};

//Gui &operator<<(Gui &gui, const std::string &msg);
