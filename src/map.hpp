#ifndef MAP_HPP_DEFINED
#define MAP_HPP_DEFINED

#include "pos.hpp"
#include <memory>
#include <boost/multi_array.hpp>
#include "tcod_util.hpp"

class Monster;
class Item;

struct Tile {
  Tile(bool discovered=false) : discovered(discovered) { }
  bool discovered;
};

class Map {
  private:
    TCODMap map;
    boost::multi_array<Tile, 2> tiles;
    int depth;
    Pos entrance_;
    Pos exit_;

    // map gen stuff
    void generate();
    void gen_rand_walk();
    bool soft_edge_limit_dir(const Pos &pos, Pos &d, int limit);
    int in_soft_limit(const Pos &pos, int limit);
    bool can_sober(const Pos &pos, const Pos &dir, int hall_len, int limit, double density_allowed);
    bool in_level(const Pos &pos);
    void generate_monster(const Pos &pos);
    void generate_item(const Pos &pos);
  public:
    Map(int w, int h, int depth);
    ~Map();
    Map(const Map &other);
    Map &operator=(const Map &other);
    TCODMap &get_map();
    std::vector<Monster> monsters;
    std::vector<Item> items;
    int get_width() const;
    int get_height() const;
    Pos entrance() const { return entrance_; }
    Pos exit() const { return exit_; }
    int is_walkable(const Pos &pos);
    void set_walkable(const Pos &pos, bool walkable);
    void draw();
    Tile &tile(const Pos &pos);
    bool in_fov(const Pos &pos, const Pos &target, bool recalculate=true);
    void calculate_fov(const Pos &src);
};

#endif //MAP_HPP_DEFINED
