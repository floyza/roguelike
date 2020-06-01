#include "map.hpp"
#include "tcod_util.hpp"
#include "game.hpp"
#include "player.hpp"
#include "monster.hpp"
#include <cassert>
#include <iostream>
#include <vector>

enum class Dir {north,west,east,south};

struct Coord {
  Coord(Map &map) : map(map) { }
  Map &map;
  int x;
  int y;
  bool move_dir(Dir d);
};

bool Coord::move_dir(Dir d) {
  switch (d) {
  case Dir::west:
    if (x>0) {
      --x;
      return true;
    }
    break;
  case Dir::east:
    if (x<map.get_width()-1) {
      ++x;
      return true;
    }
    break;
  case Dir::north:
    if (y>0) {
      --y;
      return true;
    }
    break;
  case Dir::south:
    if (y<map.get_height()-1) {
      ++y;
      return true;
    }
    break;
  }
  return false;
}

Dir rand_dir() {
  return static_cast<Dir>(rand_int(0,3));
}

Map::Map(int w, int h, int depth)
  : map(std::make_unique<TCODMap>(w,h)), tiles(boost::extents[w][h]), depth(depth)
{
  constexpr double grain_count=5;
  constexpr double filled_percent=75;

  const int granularity = std::min(std::ceil(w/grain_count), std::ceil(h/grain_count));

  int tries=0;

  while (true) {
    ++tries;
    map = std::make_unique<TCODMap>(w,h);
    monsters=std::vector<std::unique_ptr<Monster>>{};
    gen_rand_walk();

    int total_grids=0;
    int filled_grids=0;

    for (int x=0; x<w; x+=granularity) {
      for (int y=0; y<h; y+=granularity) {
	++total_grids;
	for (int x_fine=x; x_fine<x+granularity && x_fine<w; ++x_fine) {
	  for (int y_fine=y; y_fine<y+granularity && y_fine<h; ++y_fine) {
	    if (is_walkable(x_fine, y_fine)) {
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
      break;
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
  constexpr int sober_chance = 30;
  constexpr int sober_density_allowed = 35;
  constexpr double soft_edge_limit_percent = 20;
  constexpr double monster_chance = 8;
  constexpr int hall_min = 5;
  constexpr int hall_max = 10;
  constexpr int cave_min = 15;
  constexpr int cave_max = 25;
  constexpr int start_variation = 5;

  
  const int width = get_width();
  const int height = get_height();
  const int soft_edge_limit = std::min(width,height)*(soft_edge_limit_percent/100);
  int req_tiles = (width*height)*(percentage/100);

  
  int done_tiles = rand_int(cave_min, cave_max);
  
  Coord loc(*this);
  loc.x = width/2 + rand_int(-start_variation, start_variation);
  loc.y = height/2 + rand_int(-start_variation, start_variation);
  g->you->x = loc.x;
  g->you->y = loc.y;
  
  bool sober = false; // true if we are digging a corridor
  int steps = rand_int(cave_min, cave_max); // steps until change sober state
  Dir dir = rand_dir();

  bool last_room = false;

  while (true) {
    if (done_tiles >= req_tiles && !sober) {
      if (last_room)
	break;
      else
	last_room = true;
    }
    if (!is_walkable(loc.x, loc.y)) {
      set_walkable(loc.x, loc.y, true);
      ++done_tiles;
    }

    if (steps-- == 0) {
      int hall_len = rand_int(hall_min, hall_max);
      if (rand_int(1,100) <= sober_chance &&
	  can_sober(loc.x, loc.y, dir, hall_len, soft_edge_limit, sober_density_allowed) &&
	  !sober) {
	sober = true;
	steps = hall_len;
      } else {
	if (rand_int(1,100) <= monster_chance)
	  monsters.push_back(std::make_unique<Monster>('g', TCODColor::blue, *this, loc.x, loc.y));
	sober = false;
	steps = rand_int(cave_min, cave_max);
      }
    }

    if (sober && in_soft_limit(loc.x, loc.y, soft_edge_limit)) {
      sober = false;
      steps = rand_int(cave_min, cave_max);
    }

    if (!sober) {
      // turn
      Dir old_dir = dir;
      while (old_dir == dir) {
	dir = rand_dir();
      }
      int limit_tier = in_soft_limit(loc.x, loc.y, soft_edge_limit);
      if (rand_int(1,soft_edge_limit*soft_edge_limit) <= limit_tier*limit_tier)
	soft_edge_limit_dir(loc.x, loc.y, dir, soft_edge_limit);
    }

    loc.move_dir(dir);
    if (loc.x == 0)
      loc.x = 1;
    else if (loc.x == width-1)
      loc.x = get_width()-2;
    if (loc.y == 0)
      loc.y = 1;
    else if (loc.y == height-1)
      loc.y = height-2;
  }
}

bool Map::in_level(int x, int y) {
  return !(x < 0 || x > get_width()-1 || y < 0 || y > get_height()-1);
}

bool Map::soft_edge_limit_dir(int x, int y, Dir &d, int limit)
{
  bool changed = false;
  if (x < limit) {
    d = Dir::east;
    changed = true;
  } else if (x > get_width()-1 - limit) {
    d = Dir::west;
    changed = true;
  } else if (y < limit) {
    d = Dir::south;
    changed = true;
  } else if (y > get_height()-1 - limit) {
    d = Dir::north;
    changed = true;
  }
  return changed;
}

int Map::in_soft_limit(int x, int y, int limit)
{
  using std::max;
  int depth=0;
  if (x < limit)
    depth = max(depth, limit-x);
  else if (x > get_width()-1 - limit)
    depth = max(depth, limit-(get_width()-x-1));
  if (y < limit)
    depth = max(depth, limit-y);
  else if (y > get_height()-1 - limit)
    depth = max(depth, limit-(get_height()-y-1));
  return depth;
}

bool Map::can_sober(int x, int y, Dir dir, int hall_len, int limit, int density_allowed)
{
  if (in_soft_limit(x, y, limit))
    return false;

  // increase hall_len to make it odd
  const int rect_size = hall_len + !(hall_len%2);
  int rect_x = x - hall_len/2;
  int rect_y = y - hall_len/2;
  
  switch (dir) {
  case Dir::north:
    rect_y -= hall_len/2;
    break;
  case Dir::south:
    rect_y += hall_len/2;
    break;
  case Dir::west:
    rect_x -= hall_len/2;
    break;
  case Dir::east:
    rect_x += hall_len/2;
    break;
  }

  // max 25% of tiles can be floor tiles
  const int max_amount = rect_size*rect_size*(100.d/density_allowed);
  int floor_count=0;

  for (x = rect_x; x < rect_x+rect_size; ++x) {
    for (y = rect_y; y < rect_y+rect_size; ++y) {
      if (!in_level(x,y))
	return false;
      if (is_walkable(x,y))
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
  compute_fov(g->you->x, g->you->y, g->you->view_range);
  const int width = get_width(), height = get_height();
  for (int x=0; x<width; ++x) {
    for (int y=0; y<height; ++y) {
      bool can_see = false;
      bool has_seen = tiles[x][y].discovered;
      if (in_fov(x,y)) {
	can_see = true;
	if (!has_seen) {
	  tiles[x][y].discovered = true;
	  has_seen = true;
	}
      }
      if (has_seen) {
	if (is_walkable(x,y)) {
	  TCODConsole::root->setCharForeground(x, y, can_see ? floor_seen : floor_unseen);
	  TCODConsole::root->setChar(x,y,'.');
	} else {
	  TCODConsole::root->setCharForeground(x, y, can_see ? wall_seen : wall_unseen);
	  TCODConsole::root->setChar(x,y,'#');
	}
      }
    }
  }
  for (const std::unique_ptr<Monster> &mon : monsters) {
    if (in_fov(mon->x, mon->y)) {
      mon->draw();
    }
  }
}

void Map::compute_fov(int x, int y, int range) {
  map->computeFov(x, y, range);
}

bool Map::in_fov(int x, int y) {
  return map->isInFov(x,y);
}

int Map::get_width() {
  return map->getWidth();
}

int Map::get_height() {
  return map->getHeight();
}

int Map::is_walkable(int x, int y) {
  return map->isWalkable(x, y);
}

void Map::set_walkable(int x, int y, bool walkable) {
  map->setProperties(x,y,walkable,walkable);
}

Tile &Map::tile(int x, int y) {
  return tiles[x][y];
}

TCODMap *Map::get_map() {
  return map.get();
}
