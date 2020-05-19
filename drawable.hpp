#ifndef DEFINED_DRAWABLE_HPP
#define DEFINED_DRAWABLE_HPP

#include <boost/multi_array.hpp>

struct Drawable {
  typedef boost::multi_array<char, 2> draw_map;
  int x,y;
  draw_map map;
};

#endif //DEFINED_DRAWABLE_HPP
