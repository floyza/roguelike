#include "inventory_gui.hpp"
#include "player.hpp"

Inventory_gui::Inventory_gui(Pos pos, int width, int height, Player &player,
                             std::function<void()> close_callback)
    : window(pos, width, height), target(player),
      close_callback(close_callback) {}

bool Inventory_gui::handle_input(const TCOD_key_t &input) {
  switch (input.c) {
  case 'q':
    close();
    return false;
  case 'k':
    if (index > 0)
      --index;
    break;
  case 'j':
    if (index < target.inven_size() - 1)
      ++index;
    break;
  }
  if (input.vk == TCODK_ENTER) {
    bool &eq = target.inven_item(index).equipped;
    eq = !eq;
  }
  draw();
  return true;
}

void Inventory_gui::update_window() {
  window->clear();
  using namespace std::string_literals;
  window->print(0, index, ">"s);
  for (int i = 0; i < target.inven_size(); ++i) {
    const Inven_item &item = target.inven_item(i);
    window->print(1, i, item.equipped ? "o"s : "-"s);
    window->print(3, i, item.item.name());
  }
}

void Inventory_gui::draw() {
  update_window();
  window.blit();
}

void Inventory_gui::close() { close_callback(); }
