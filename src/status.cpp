#include "status.hpp"
#include "effect.hpp"
#include "game.hpp"
#include "lua.hpp"
#include "player.hpp"

Status::Status(int id) : Status(game->lua_manager->get_status(id)) {}

Status::Status(const std::string &name)
    : Status(game->lua_manager->get_status(name)) {}

Status::Status(const Lua_status &base)
    : effect(base.type, base.func), duration(base.duration), name_(base.name),
      id_(base.id) {}

Status::~Status() = default;

Trigger Status::get_trigger() { return effect.get_trigger(); }

void Status::remove() { game->you->remove_status(id_); }

void Status::tick() {
  if (--duration == 0) {
    remove();
  }
}

int Status::id() const { return id_; }

const std::string &Status::name() const { return name_; }
