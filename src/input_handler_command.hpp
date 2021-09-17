#ifndef __INPUT_HANDLER_COMMAND_H_
#define __INPUT_HANDLER_COMMAND_H_

#include "input_handler.hpp"
#include "player.hpp"

// we need this header separate because we
// need to include input_handler.hpp here,
// and we need to remove the circular
// dependancy

class Inventory_input_handler_command : public Command {
private:
  Player *target;
  std::function<void()> callback;

public:
  Inventory_input_handler_command(Player &target,
                                  std::function<void()> callback)
      : target{&target}, callback{callback} {}
  Inventory_input_handler_command(const Inventory_input_handler_command &c)
      : target{c.target}, callback{c.callback} {}
  Inventory_input_handler_command &
  operator=(const Inventory_input_handler_command &c) {
    target = c.target;
    callback = c.callback;
    return *this;
  }
  int execute() override {
    target->push_input_handler(
        std::make_unique<Inventory_input_handler>(*target, callback));
    return 0;
  }
  std::unique_ptr<Command> clone() override {
    return std::make_unique<Inventory_input_handler_command>(*this);
  }
};

class Lua_input_handler_command : public Command {
private:
  Player *target;
  std::function<void()> callback;

public:
  Lua_input_handler_command(Player &target, std::function<void()> callback)
      : target{&target}, callback{callback} {}
  Lua_input_handler_command(const Lua_input_handler_command &c)
      : target{c.target}, callback{c.callback} {}
  Lua_input_handler_command &operator=(const Lua_input_handler_command &c) {
    target = c.target;
    callback = c.callback;
    return *this;
  }
  int execute() override {
    target->push_input_handler(std::make_unique<Lua_input_handler>(callback));
    return 0;
  }
  std::unique_ptr<Command> clone() override {
    return std::make_unique<Lua_input_handler_command>(*this);
  }
};

#endif // __INPUT_HANDLER_COMMAND_H_
