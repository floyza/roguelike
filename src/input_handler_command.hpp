#ifndef __INPUT_HANDLER_COMMAND_H_
#define __INPUT_HANDLER_COMMAND_H_

#include "player.hpp"
#include "input_handler.hpp"

// we need this header separate because we
// need to include input_handler.hpp here,
// and we need to remove the circular
// dependancy

class Inventory_input_handler_command : public Command {
  private:
    Player &target;
    std::function<void()> callback;
  public:
    Inventory_input_handler_command(Player &target, std::function<void()> callback)
      : target{target}, callback{callback}
    {}
    int execute() override {
      target.push_input_handler(std::make_unique<Inventory_input_handler>(target,callback));
      return 0;
    }
    std::unique_ptr<Command> clone() override { return std::make_unique<Inventory_input_handler_command>(*this); }
};

class Lua_input_handler_command : public Command {
  private:
    Player &target;
    std::function<void()> callback;
  public:
    Lua_input_handler_command(Player &target, std::function<void()> callback)
      : target{target}, callback{callback}
    {
    }
    int execute() override {
      target.push_input_handler(std::make_unique<Lua_input_handler>(callback));
      return 0;
    }
    std::unique_ptr<Command> clone() override { return std::make_unique<Lua_input_handler_command>(*this); }
};

#endif // __INPUT_HANDLER_COMMAND_H_
