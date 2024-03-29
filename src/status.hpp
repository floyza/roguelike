#ifndef DEFINED_STATUS_HPP
#define DEFINED_STATUS_HPP

#include <string>

#include "effect.hpp"
#include "lua.hpp"

struct Lua_status;

class Status {
  Effect effect;
  int duration;
  std::string name_;
  int id_;

public:
  Status(int id);
  Status(const std::string &name);
  Status(const Lua_status &base);

  template <Trigger trigger, typename... Args> void call_effect(Args... args);

  Trigger get_trigger();
  void tick();
  void remove();

  int id() const;
  const std::string &name() const;

  ~Status();
};

template <Trigger trigger, typename... Args>
void Status::call_effect(Args... args) {
  // we have to take a template argument of our trigger because it needs
  // to be constexpr
  effect.call_effect<trigger>(args...);
}

#endif // DEFINED_STATUS_HPP
