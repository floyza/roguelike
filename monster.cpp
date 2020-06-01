#include "monster.hpp"
#include "map.hpp"
#include "game.hpp"
#include "player.hpp"
#include "gui.hpp"

Monster::Monster(char icon, const TCODColor &color, Map &parent, const std::string &name, int max_hp, int attack, int x, int y)
  : Creature(icon, color, max_hp, attack, x, y), name_(name), parent(parent)
{
}

void Monster::do_move() {
  if (parent.in_fov(x,y)) {
    // the player can see us, so we can see them
    TCODPath path(parent.get_map());
    path.compute(x, y, g->you->x, g->you->y);
    path.get(0,&x,&y);
  } 
}

void Monster::do_attack(Creature &target) {
  target.take_damage(attack, *this);
}

void Monster::take_damage(int amount, Player &source) {
  g->msg_log->send_msg({"You attack the " + name() + "!"});
  g->msg_log->send_nl();
  hp -= amount;
}

void Monster::take_damage(int amount, Monster &source) {
  g->msg_log->send_msg({"The " + source.name() + " attacks the " + name() + "!"});
  g->msg_log->send_nl();
  hp -= amount;
}
