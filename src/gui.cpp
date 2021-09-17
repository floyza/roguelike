#include "gui.hpp"
#include "tcod_util.hpp"
#include <sstream>

Gui::Gui(int x, int y, int w, int h) : con(Pos{x, y}, w, h) {}

void Gui::draw() {
  draw_msgs();
  con.blit();
}

void Gui::draw_msgs() {
  con->clear();
  for (std::size_t i = 0; i < lines.size(); ++i) {
    const Message &line_msg = lines[i];
    con->setDefaultForeground(line_msg.color);
    int x;
    if (line_msg.center) {
      con->setAlignment(TCOD_CENTER);
      x = con->getWidth() / 2;
    } else {
      con->setAlignment(TCOD_LEFT);
      x = 1;
    }
    con->print(x, i, line_msg.text);
  }
}

void Gui::add_line(const Message &msg) {
  lines.push_back(msg);
  if (lines.size() > static_cast<std::size_t>(con->getHeight()))
    lines.pop_front();
}

Gui &Gui::send_nl() {
  add_line({""});
  return *this;
}

Gui &Gui::send_msg(const Message &msg) {
  // split the message into lines
  std::string line;
  std::string word;
  std::istringstream msg_stream{msg.text};
  while (msg_stream >> word) {
    std::string next_line;
    if (line.size() > 0)
      next_line = line + ' ' + word;
    else
      next_line = word;
    if (next_line.size() >= static_cast<std::size_t>(con->getWidth() - 2)) {
      add_line({line, msg.color, msg.center});
      line = word;
    } else {
      line = next_line;
    }
  }
  if (line.size() > 0)
    add_line({line, msg.color, msg.center});

  return *this;
}

Message::Message(const std::string &text, const TCODColor &color, bool center)
    : text(text), color(color), center(center) {}
