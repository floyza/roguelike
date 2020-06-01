#include "monster.hpp"
#include "map.hpp"
#include "game.hpp"
#include "player.hpp"

Monster::Monster(char icon, const TCODColor &color, Map &parent, int x, int y)
  : Creature(icon, color, x, y), parent(parent)
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
