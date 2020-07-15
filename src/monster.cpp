#include "monster.hpp"
#include "map.hpp"
#include "game.hpp"
#include "player.hpp"
#include "gui.hpp"
#include "item.hpp"
#include "lua.hpp"
#include <algorithm>

Monster::Monster(int id, Map &parent, int x, int y)
  : Monster(game->lua_manager->get_mon(id), parent, x, y)
{
}

Monster::Monster(const std::string &name, Map &parent, int x, int y)
  : Monster(game->lua_manager->get_mon(name), parent, x, y)
{
}

Monster::Monster(const Lua_monster &base, Map &parent, int x, int y)
  : Creature(base.icon, base.color, base.max_hp, base.attack, x, y), name_(base.name), id_(base.id), parent(&parent)
{
}

Monster::~Monster() = default;

std::pair<int, int> Monster::step_to_dest() {
  // first try to walk around monsters
  // if this doesn't work, walk as if the
  // other monsters dont exist
  std::pair<int,int> step(x,y);
  if (dest_x != -1 && dest_y != -1) {
    TCODMap map(parent->get_map()->getWidth(), parent->get_map()->getHeight());
    map.copy(parent->get_map());
    for (const Monster &monster : parent->monsters) {
      int mon_x = monster.x, mon_y = monster.y;
      map.setProperties(mon_x, mon_y, map.isWalkable(mon_x, mon_y), false);
    }
    TCODPath path(&map);
    path.compute(x, y, dest_x, dest_y);

    if (!path.isEmpty()) {
      path.get(0, &step.first, &step.second);
    } else {
      // still try to walk towards the player
      TCODPath path_closer(parent->get_map());
      path_closer.compute(x, y, dest_x, dest_y);
      int pstep_x, pstep_y;
      if (!path_closer.isEmpty()) {
	path_closer.get(0, &pstep_x, &pstep_y);
	bool can_step = true;
	for (const Monster &monster : parent->monsters) {
	  if (monster.x == pstep_x && monster.y == pstep_y) {
	    can_step = false;
	    break;
	  }
	}
	if (can_step) {
	  step.first = pstep_x;
	  step.second = pstep_y;
	}
      }
    }
  }
  return step;
}

void Monster::do_move() {
  if (parent->in_fov(x,y)) {
    // the player can see us, so we can see them
    dest_x = game->you->x;
    dest_y = game->you->y;
  }

  std::pair<int,int> step = step_to_dest();

  if (game->you->x == step.first && game->you->y == step.second) {
    do_attack(*game->you);
  } else {
    x = step.first;
    y = step.second;
  }
}

void Monster::do_attack(Creature &target) {
  target.take_damage(attack, *this);
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
  game->you->call_triggers<Trigger::ON_KILL>(std::ref(*this));
  game->send_msg({"The " + name() + " dies!"});
  auto iter = std::remove_if(parent->monsters.begin(), parent->monsters.end(), [this](const Monster &mon){return this==&mon;});
  parent->monsters.erase(iter, parent->monsters.end());
}

int Monster::id() const { return id_; }

const std::string &Monster::name() const { return name_; }
