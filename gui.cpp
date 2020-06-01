#include "gui.hpp"
#include "tcod_util.hpp"
#include <sstream>

Gui::Gui(int x, int y, int w, int h)
  : con(std::make_unique<TCODConsole>(w, h)), rootx(x), rooty(y)
{
}

void Gui::draw() {
  draw_border();
  draw_msgs();
  TCODConsole::blit(con.get(), 0,0,0,0, TCODConsole::root, rootx, rooty);
}

void Gui::draw_border() {
  const int width = con->getWidth();
  const int height = con->getHeight();
  
  for (int x=1; x<width-1; ++x) {
    con->putChar(x, 0, wall_merge(TCOD_CHAR_HLINE, TCODConsole::root->getChar(x+rootx, rooty)));
    con->putChar(x, height-1, wall_merge(TCOD_CHAR_HLINE, TCODConsole::root->getChar(x+rootx, rooty+height-1)));
  }
  for (int y=1; y<height-1; ++y) {
    con->putChar(0, y, wall_merge(TCOD_CHAR_VLINE, TCODConsole::root->getChar(rootx, rooty+y)));
    con->putChar(width-1, y, wall_merge(TCOD_CHAR_VLINE, TCODConsole::root->getChar(rootx+width-1, rooty+y)));
  }
  con->putChar(0, 0, wall_merge(TCOD_CHAR_NW, TCODConsole::root->getChar(rootx, rooty)));
  con->putChar(width-1, 0, wall_merge(TCOD_CHAR_NE, TCODConsole::root->getChar(rootx+width-1, rooty+0)));
  con->putChar(0, height-1, wall_merge(TCOD_CHAR_SW, TCODConsole::root->getChar(rootx, rooty+height-1)));
  con->putChar(width-1, height-1, wall_merge(TCOD_CHAR_SE, TCODConsole::root->getChar(rootx+width-1, rooty+height-1)));
}

void Gui::draw_msgs() {
  for (std::size_t i=0; i<lines.size(); ++i) {
    const Message &line_msg = lines[i];
    con->setDefaultForeground(line_msg.color);
    int x;
    if (line_msg.center) {
      con->setAlignment(TCOD_CENTER);
      x = con->getWidth()/2;
    } else {
      con->setAlignment(TCOD_LEFT);
      x = 1;
    }
    con->print(x, i+1, line_msg.text.c_str());
  }
}

void Gui::add_line(const Message &msg) {
  lines.push_back(msg);
  if (lines.size() > static_cast<std::size_t>(con->getHeight()-2))
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
    if (next_line.size() >= static_cast<std::size_t>(con->getWidth()-2)) {
      add_line({line, msg.color, msg.center});
      line = word;
    } else {
      line = next_line;
    }
  }
  if (line.size() > 0)
    add_line({line, msg.color, msg.center});
  //  con->rect(1,1, con->getWidth()-2, con->getHeight()-2, true);
  
  return *this;
}

Message::Message(const std::string& text, const TCODColor &color, bool center)
  : text(text), color(color), center(center)
{
}
