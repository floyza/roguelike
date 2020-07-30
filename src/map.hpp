#ifndef MAP_HPP_DEFINED
#define MAP_HPP_DEFINED

#include <memory>
#include <boost/multi_array.hpp>

class TCODMap;
class Monster;
class Item;
enum class Dir;

struct Tile {
  Tile(bool discovered=false) : discovered(discovered) { }
  bool discovered;
};

class Map {
  std::unique_ptr<TCODMap> map;
  boost::multi_array<Tile, 2> tiles;
  int depth;

  // map gen stuff
  void gen_rand_walk();
  bool soft_edge_limit_dir(int x, int y, Dir &d, int limit);
  int in_soft_limit(int x, int y, int limit);
  bool can_sober(int x, int y, Dir dir, int hall_len, int limit, int density_allowed);
  bool in_level(int x, int y);
  void generate_monster(int x, int y);
  void generate_item(int x, int y);
public:
  Map(int w, int h, int depth);
  ~Map();
  TCODMap *get_map();
  std::vector<Monster> monsters;
  std::vector<Item> items;
  int get_width();
  int get_height();
  int is_walkable(int x, int y);
  void set_walkable(int x, int y, bool walkable);
  void draw();
  Tile &tile(int x, int y);
  bool in_fov(int posx, int posy, int x, int y);
};

#endif //MAP_HPP_DEFINED
