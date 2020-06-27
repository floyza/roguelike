#include "status.hpp"
#include "effect.hpp"
#include "lua.hpp"
#include "game.hpp"
#include "player.hpp"

Status::Status(const std::string &id)
{
  const Lua_status &status_base = game->get_status(id);
  name = status_base.name;
  effect = Effect{status_base.type, status_base.func};
  duration = status_base.duration;
}

Status::~Status() = default;

Trigger Status::get_trigger() { return effect.get_trigger(); }

void Status::remove() {
  game->you->remove_status(name);
}

void Status::tick() {
  if (--duration == 0) {
    remove();
  }
}
