#ifndef MAP_HPP_DEFINED
#define MAP_HPP_DEFINED

#include <memory>
#include "tcod.hpp"
#include "drawable.hpp"

class Map {
  TCODMap map;
  int depth;
public:
  Map(int w, int h, int depth);
  Drawable get_drawable();
};

#endif //MAP_HPP_DEFINED
