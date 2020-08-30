#include "map.hpp"
#include "tcod_util.hpp"
#include "game.hpp"
#include "player.hpp"
#include "monster.hpp"
#include "item.hpp"
#include "pos.hpp"
#include "dir.hpp"
#include <cassert>
#include <iostream>
#include <vector>

Pos rand_dir() {
  switch(rand_int(0,3)) {
    case 0:
      return Dir::n;
    case 1:
      return Dir::s;
    case 2:
      return Dir::w;
    case 3:
      return Dir::e;
  }
  throw std::runtime_error { "rand_int returns invalid int" };
}

Map::Map(int w, int h, int depth)
  : map(std::make_unique<TCODMap>(w,h)), tiles(boost::extents[w][h]), depth(depth)
{
  constexpr double grain_count=5;
  constexpr double filled_percent=75;

  const int granularity = std::min(std::ceil(w/grain_count), std::ceil(h/grain_count));

  int tries=0;

  bool generating = true;
  while (generating) {
    ++tries;
    map = std::make_unique<TCODMap>(w,h);
    monsters=std::vector<Monster>{};
    gen_rand_walk();

    int total_grids=0;
    int filled_grids=0;

    for (int x=0; x<w; x+=granularity) {
      for (int y=0; y<h; y+=granularity) {
        ++total_grids;
        for (int x_fine=x; x_fine<x+granularity && x_fine<w; ++x_fine) {
          for (int y_fine=y; y_fine<y+granularity && y_fine<h; ++y_fine) {
            if (is_walkable(Pos{x_fine, y_fine})) {
              ++filled_grids;
              // break out of x_fine and y_fine loops
              x_fine=w;
              break;
            }
          }
        }
      }
    }

    if (total_grids*(filled_percent/100) < filled_grids)
      generating = false;
  }
  std::cerr << "map gen done in " << tries << " try(s)\n";
}

Map::~Map() = default;

void Map::gen_rand_walk() {
  // Map generation done with modified 'drunkard's walk' algorithm
  // Basic algorithm described here: http://www.roguebasin.com/index.php?title=Random_Walk_Cave_Generation
  // With some modifications described here: https://forums.roguetemple.com//index.php?topic=4128.0

  // tweakable variables:
  constexpr double percentage = 30;
  constexpr double sober_chance = 30;
  constexpr double sober_density_allowed = 35;
  constexpr double soft_edge_limit_percent = 20;
  constexpr double monster_chance = 2;
  constexpr double item_chance = .5;
  constexpr int hall_min = 5;
  constexpr int hall_max = 10;
  constexpr int cave_min = 15;
  constexpr int cave_max = 25;
  constexpr int start_variation = 5;

  
  const int width = get_width();
  const int height = get_height();
  const int soft_edge_limit = std::min(width,height)*(soft_edge_limit_percent/100);
  const int req_tiles = (width*height)*(percentage/100);
  const int max_steps = req_tiles*500;

  
  int done_tiles = rand_int(cave_min, cave_max);
  int total_steps = 0;
  
  Pos loc;
  loc.x = width/2 + rand_int(-start_variation, start_variation);
  loc.y = height/2 + rand_int(-start_variation, start_variation);
  game->you->pos = loc;
  
  bool sober = false; // true if we are digging a corridor
  int steps = rand_int(cave_min, cave_max); // steps until change sober state
  Pos dir = rand_dir();

  bool last_room = false;

  while (true) {
    if (done_tiles >= req_tiles && !sober) {
      if (last_room)
        break;
      else
        last_room = true;
    }
    if (!is_walkable(loc)) {
      set_walkable(loc, true);
      ++done_tiles;
    }

    if (steps-- == 0) {
      int hall_len = rand_int(hall_min, hall_max);
      if (percent_chance(sober_chance) &&
          can_sober(loc, dir, hall_len, soft_edge_limit, sober_density_allowed) &&
          !sober) {
        sober = true;
        steps = hall_len;
      } else {
        if (percent_chance(monster_chance)) {
          generate_monster(loc);
        }
        if (percent_chance(item_chance)) {
          generate_item(loc);
        }
        sober = false;
        steps = rand_int(cave_min, cave_max);
      }
    }

    if (sober && in_soft_limit(loc, soft_edge_limit)) {
      sober = false;
      steps = rand_int(cave_min, cave_max);
    }

    if (!sober) {
      // turn
      Pos old_dir = dir;
      while (old_dir == dir) {
        dir = rand_dir();
      }
      int limit_tier = in_soft_limit(loc, soft_edge_limit);
      if (rand_int(1,soft_edge_limit*soft_edge_limit) <= limit_tier*limit_tier)
        soft_edge_limit_dir(loc, dir, soft_edge_limit);
    }

    loc += dir;
    if (loc.x == 0)
      loc.x = 1;
    else if (loc.x == width-1)
      loc.x = get_width()-2;
    if (loc.y == 0)
      loc.y = 1;
    else if (loc.y == height-1)
      loc.y = height-2;

    if (++total_steps > max_steps) {
      std::cerr << "map generation exceeded max_steps\n";
      break;
    }
  }
}

bool Map::in_level(const Pos &pos) {
  return !(pos.x < 0 || pos.x > get_width()-1 || pos.y < 0 || pos.y > get_height()-1);
}

bool Map::soft_edge_limit_dir(const Pos &pos, Pos &d, int limit)
{
  bool changed = false;
  if (pos.x < limit) {
    d = Dir::e;
    changed = true;
  } else if (pos.x > get_width()-1 - limit) {
    d = Dir::w;
    changed = true;
  } else if (pos.y < limit) {
    d = Dir::s;
    changed = true;
  } else if (pos.y > get_height()-1 - limit) {
    d = Dir::n;
    changed = true;
  }
  return changed;
}

int Map::in_soft_limit(const Pos &pos, int limit)
{
  using std::max;
  int depth=0;
  if (pos.x < limit)
    depth = max(depth, limit-pos.x);
  else if (pos.x > get_width()-1 - limit)
    depth = max(depth, limit-(get_width()-pos.x-1));
  if (pos.y < limit)
    depth = max(depth, limit-pos.y);
  else if (pos.y > get_height()-1 - limit)
    depth = max(depth, limit-(get_height()-pos.y-1));
  return depth;
}

bool Map::can_sober(const Pos &pos, const Pos &dir, int hall_len, int limit, double density_allowed)
{
  if (in_soft_limit(pos, limit))
    return false;

  // increase hall_len to make it odd
  const int rect_size = hall_len + !(hall_len%2);
  Pos rect = {
  (pos.x - hall_len / 2) + dir.x * (hall_len / 2),
  (pos.y - hall_len / 2) + dir.y * (hall_len / 2) };

  // max 25% of tiles can be floor tiles
  const int max_amount = rect_size * rect_size * (density_allowed / 100);
  int floor_count=0;

  Pos curr=rect;

  for (; curr.x < rect.x+rect_size; ++curr.x) {
    for (; curr.y < rect.y+rect_size; ++curr.y) {
      if (!in_level(curr))
        return false;
      if (is_walkable(curr))
        ++floor_count;
      if (floor_count > max_amount)
        return false;
    }
  }
  return true;
}

void Map::draw()
{
  TCODColor wall_seen(0x66,0x66,0x00);
  TCODColor wall_unseen(0x40,0x40,0x00);
  TCODColor floor_seen(0xff,0xff,0xff);
  TCODColor floor_unseen(0x9e,0x9e,0x9e);
  const int width = get_width(), height = get_height();
  calculate_fov(game->you->pos);
  for (int x=0; x<width; ++x) {
    for (int y=0; y<height; ++y) {
      bool can_see = false;
      bool has_seen = tiles[x][y].discovered;
      if (in_fov(game->you->pos, Pos{x, y}, false)) {
        can_see = true;
        if (!has_seen) {
          tiles[x][y].discovered = true;
          has_seen = true;
        }
      }
      if (has_seen) {
        if (is_walkable(Pos{x, y})) {
          TCODConsole::root->setCharForeground(x, y, can_see ? floor_seen : floor_unseen);
          TCODConsole::root->setChar(x,y,'.');
        } else {
          TCODConsole::root->setCharForeground(x, y, can_see ? wall_seen : wall_unseen);
          TCODConsole::root->setChar(x,y,'#');
        }
      }
    }
  }
  for (const Item &item : items) {
    if (in_fov(game->you->pos, item.pos, false)) {
      item.draw();
    }
  }
  for (const Monster &mon : monsters) {
    if (in_fov(game->you->pos, mon.pos, false)) {
      mon.draw();
    }
  }
}

void Map::generate_monster(const Pos &pos) {
  monsters.emplace_back(game->lua_manager->get_rand_mon(depth), *this, pos);
}

void Map::generate_item(const Pos &pos) {
  items.emplace_back(game->lua_manager->get_rand_item(depth), pos);
}

void Map::calculate_fov(const Pos &src) {
  map->computeFov(src.x, src.y, 9, true, FOV_PERMISSIVE_3);
}

bool Map::in_fov(const Pos &pos, const Pos &target, bool recalculate) {
  if (recalculate)
    calculate_fov(pos);
  return map->isInFov(target.x,target.y);
}

int Map::get_width() {
  return map->getWidth();
}

int Map::get_height() {
  return map->getHeight();
}

int Map::is_walkable(const Pos &pos) {
  return map->isWalkable(pos.x, pos.y);
}

void Map::set_walkable(const Pos &pos, bool walkable) {
  map->setProperties(pos.x,pos.y,walkable,walkable);
}

Tile &Map::tile(const Pos &pos) {
  return tiles[pos.x][pos.y];
}

TCODMap *Map::get_map() {
  return map.get();
}
