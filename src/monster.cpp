#include "monster.hpp"
#include "map.hpp"
#include "game.hpp"
#include "player.hpp"
#include "gui.hpp"
#include "item.hpp"
#include "lua.hpp"
#include <algorithm>

Monster::Monster(int id, int parent, const Pos &pos)
  : Monster(game->lua_manager->get_mon(id), parent, pos)
{
}

Monster::Monster(const std::string &name, int parent, const Pos &pos)
  : Monster(game->lua_manager->get_mon(name), parent, pos)
{
}

Monster::Monster(const Lua_monster &base, int parent, const Pos &pos)
  : Creature(base.icon, base.color, base.max_hp, base.attack, pos), name_(base.name), id_(base.id), parent_index(parent)
{
}

Monster::~Monster() = default;

Pos Monster::step_to_dest() {
  // first try to walk around monsters
  // if this doesn't work, walk as if the
  // other monsters dont exist
  Pos step = pos;
  if (dest.x != -1 && dest.y != -1) {
    TCODMap map(parent().get_map().getWidth(), parent().get_map().getHeight());
    map.copy(&parent().get_map());
    for (const Monster &monster : parent().monsters) {
      map.setProperties(monster.pos.x, monster.pos.y, map.isWalkable(monster.pos.x, monster.pos.y), false);
    }
    TCODPath path(&map);
    path.compute(pos.x, pos.y, dest.x, dest.y);

    if (!path.isEmpty()) {
      path.get(0, &step.x, &step.y);
    } else {
      // still try to walk towards the player
      TCODPath path_closer(&parent().get_map());
      path_closer.compute(pos.x, pos.y, dest.x, dest.y);
      Pos pstep;
      if (!path_closer.isEmpty()) {
	path_closer.get(0, &pstep.x, &pstep.y);
	step = pstep;
      }
    }
  }
  return step;
}

void Monster::do_turn() {
  if (parent().in_fov(pos, game->you->pos)) {
    // the player can see us, so we can see them
    dest = game->you->pos;
  }

  do_move(step_to_dest());
}

void Monster::do_attack(Creature &target) {
  target.take_damage(attack, *this);
}

bool Monster::do_move(const Pos &new_pos) {
  for (const Monster &monster : parent().monsters) {
    if (monster.pos == new_pos) {
      return true;
    }
  }

  if (game->you->pos == new_pos) {
    do_attack(*game->you);
  } else {
    pos = new_pos;
  }

  return true;
}

void Monster::take_damage(int amount, Player &source) {
  (void)source; // we aren't using the reference to player for now
  // we will later once we implement effects on monsters
  game->send_msg({"You attack the " + name() + " for " + std::to_string(amount) + " damage!"});
  hp -= amount;
  if (hp<=0)
    die();
}

void Monster::take_damage(int amount, Monster &source) {
  game->send_msg({"The " + source.name() + " attacks the " + name() + " for " + std::to_string(amount) + " damage!"});
  hp -= amount;
  if (hp<=0)
    die();
}

void Monster::die() {
  dead_ = true;
}

void Monster::push_death() {
  if (dead_) {
    game->you->call_triggers<Trigger::ON_KILL>(std::ref(*this));
    game->send_msg({"The " + name() + " dies!"});
    auto iter = std::remove_if(parent().monsters.begin(), parent().monsters.end(), [this](const Monster &mon){return this==&mon;});
    parent().monsters.erase(iter, parent().monsters.end());
  }
}

int Monster::id() const { return id_; }

const std::string &Monster::name() const { return name_; }

Map &Monster::parent() {
  // when we are active, we are on the right level
  return game->map();
}

const Map &Monster::parent() const {
  return game->map();
}
