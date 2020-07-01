#ifndef DEFINED_TCOD_HPP
#define DEFINED_TCOD_HPP

#include <memory>
#include <libtcod/libtcod.hpp>
#include <iterator>
#include <type_traits>

extern std::unique_ptr<TCODRandom> tcod_rand;
int rand_int(int min, int max);
int rand_int_log(int min, int max);
bool percent_chance(double chance);
int wall_merge(int wall1, int wall2);

template <typename Iter>
Iter random_element(Iter begin, Iter end) {
  std::advance(begin, rand_int(0, std::distance(begin, end)-1));
  return begin;
}

#endif //DEFINED_TCOD_HPP
