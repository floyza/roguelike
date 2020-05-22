#include "map.hpp"
#include "tcod.hpp"
#include "game.hpp"
#include "player.hpp"
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
  : map(std::make_unique<TCODMap>(w,h)), depth(depth)
{
  // divide the map into four pieces and make sure there are enough walkable
  // spaces in each corner, else redo
  constexpr double percent_req=10;
  const int mid_x=w/2;
  const int mid_y=h/2;
  const int piece_size = mid_x*mid_y;
  const int piece_count = piece_size*(percent_req/100);
  int count_nw=0,count_ne=0,count_sw=0,count_se=0;

  int trys=0;
  while (count_nw < piece_count || count_ne < piece_count || count_sw < piece_count || count_se < piece_count) {
    ++trys;
    map = std::make_unique<TCODMap>(w,h);
    gen_rand_walk();

    count_nw=0;count_ne=0;count_sw=0;count_se=0;
    for (int x=0; x<mid_x; ++x)
      for (int y=0; y<mid_y; ++y)
	count_nw += is_walkable(x,y);
    for (int x=mid_x; x<w; ++x)
      for (int y=0; y<mid_y; ++y)
	count_ne += is_walkable(x,y);
    for (int x=0; x<mid_x; ++x)
      for (int y=mid_y; y<h; ++y)
	count_sw += is_walkable(x,y);
    for (int x=mid_x; x<w; ++x)
      for (int y=mid_y; y<h; ++y)
	count_se += is_walkable(x,y);
  }
  std::cerr << "map gen done in " << trys << " trys\n";
}

void Map::gen_rand_walk() {
  // Map generation done with modified 'drunkard's walk' algorithm
  // Basic algorithm described here: http://www.roguebasin.com/index.php?title=Random_Walk_Cave_Generation
  // Some modification ideas here: https://forums.roguetemple.com//index.php?topic=4128.0

  // tweakable variables:
  constexpr double percentage = 25;
  constexpr int sober_chance = 40;
  constexpr int sober_density_allowed = 35;
  constexpr int soft_edge_limit = 10;
  constexpr int hall_min = 5;
  constexpr int hall_max = 10;
  constexpr int cave_min = 20;
  constexpr int cave_max = 30;
  constexpr int start_variation = 5;

  
  const int width = get_width();
  const int height = get_height();
  int req_tiles = (width*height)*(percentage/100);

  
  int done_tiles = rand_int(cave_min, cave_max);
  
  Coord loc(*this);
  int start_x = width/2 + rand_int(-start_variation, start_variation);
  int start_y = height/2 + rand_int(-start_variation, start_variation);
  loc.x = start_x;
  loc.y = start_y;
  
  bool sober = false; // true if we are digging a corridor
  int steps = 0; // steps until change sober state
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
    // if (loc.x == 0)
    //   loc.x = 1;
    // else if (loc.x == width-1)
    //   loc.x = get_width()-2;
    // if (loc.y == 0)
    //   loc.y = 1;
    // else if (loc.y == height-1)
    //   loc.y = height-2;
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
  const int width = get_width(), height = get_height();
  for (int x=0; x<width; ++x) {
    for (int y=0; y<height; ++y) {
      if (is_walkable(x,y)) {
	TCODConsole::root->setChar(x,y,' ');
      } else {
	TCODConsole::root->setChar(x,y,'#');
      }
    }
  }
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
