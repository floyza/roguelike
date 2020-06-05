#include "monster.hpp"
#include "map.hpp"
#include "game.hpp"
#include "player.hpp"
#include "gui.hpp"
#include "mon_id.hpp"
#include "item.hpp"
#include <algorithm>

Monster::Monster(const mon_id &id, Map &parent, int x, int y)
  : Creature(id.icon, id.color, id.max_hp, id.attack, x, y), name_(id.name), parent(parent)
{
}

Monster::~Monster() = default;

std::pair<int, int> Monster::step_to_dest() {
  // first try to walk around monsters
  // if this doesn't work, walk as if the
  // other monsters dont exist
  TCODMap map(parent.get_map()->getWidth(), parent.get_map()->getHeight());
  map.copy(parent.get_map());
  for (const auto &monster : parent.monsters) {
    int mon_x = monster->x, mon_y = monster->y;
    map.setProperties(mon_x, mon_y, map.isWalkable(mon_x, mon_y), false);
  }
  TCODPath path(&map);
  path.compute(x, y, g->you->x, g->you->y);

  std::pair<int,int> step(x,y);
  if (!path.isEmpty()) {
    path.get(0, &step.first, &step.second);
  } else {
    // still try to walk towards the player
    TCODPath path_closer(parent.get_map());
    path_closer.compute(x, y, g->you->x, g->you->y);
    int pstep_x, pstep_y;
    if (!path_closer.isEmpty()) {
      path_closer.get(0, &pstep_x, &pstep_y);
      bool can_step = true;
      for (const auto &monster : parent.monsters) {
	if (monster->x == pstep_x && monster->y == pstep_y) {
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
  return step;
}

void Monster::do_move() {
  if (parent.in_fov(x,y)) {
    // the player can see us, so we can see them
    dest_x = g->you->x;
    dest_y = g->you->y;
  }

  std::pair<int,int> step = step_to_dest();

  if (g->you->x == step.first && g->you->y == step.second) {
    do_attack(*g->you);
  } else {
    x = step.first;
    y = step.second;
  }
}

void Monster::do_attack(Creature &target) {
  target.take_damage(attack, *this);
}

void Monster::take_damage(int amount, Player &source) {
  g->msg_log->send_msg({"You attack the " + name() + " for " + std::to_string(amount) + " damage!"});
  g->msg_log->send_nl();
  hp -= amount;
}

void Monster::take_damage(int amount, Monster &source) {
  g->msg_log->send_msg({"The " + source.name() + " attacks the " + name() + " for " + std::to_string(amount) + " damage!"});
  g->msg_log->send_nl();
  hp -= amount;
  if (hp<=0)
    die();
}

void Monster::die() {
  g->you->call_triggers_generic(Trigger::ON_HIT);
  static auto pred = [this](const auto &val){ return val.get() == this; };
  auto iter = std::remove_if(parent.monsters.begin(), parent.monsters.end(), pred);
  parent.monsters.erase(iter, parent.monsters.end());
}
