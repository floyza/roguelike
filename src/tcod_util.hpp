#ifndef DEFINED_TCOD_HPP
#define DEFINED_TCOD_HPP

#include <algorithm>
#include <functional>
#include <memory>
#include <libtcod/libtcod.hpp>
#include <iterator>
#include <type_traits>

extern std::unique_ptr<TCODRandom> tcod_rand;
int rand_int(int min, int max);
int rand_int_log(int min, int max);
int rand_int_log_bounded(int min, int max, int bmin, int bmax);
bool percent_chance(double chance);
int wall_merge(int wall1, int wall2);

template <typename Iter>
Iter random_element(Iter begin, Iter end) {
  std::advance(begin, rand_int(0, std::distance(begin, end)-1));
  return begin;
}


inline void hash_combine(std::size_t& /*seed*/) { }

template <typename T, typename... Rest>
inline void hash_combine(std::size_t& seed, const T& v, Rest... rest) {
  std::hash<T> hasher;
  seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
  hash_combine(seed, rest...);
}

namespace std {
  template <> struct hash<TCOD_key_t>
  {
    size_t operator()(const TCOD_key_t& val) const
    {
      std::size_t hash = 0;
      hash_combine(hash, val.vk, val.c, val.pressed, val.lalt, val.lctrl, val.ralt, val.rctrl, val.shift);
      return hash;
    }
  };
  template <> struct equal_to<TCOD_key_t>
  {
    size_t operator()(const TCOD_key_t& v1, const TCOD_key_t &v2) const
    {
      return v1.vk==v2.vk && v1.c==v2.c && v1.pressed==v2.pressed && v1.lalt==v2.lalt
	&& v1.lctrl==v2.lctrl && v1.ralt==v2.ralt && v1.rctrl==v2.rctrl && v1.shift==v2.shift;
    }
  };
}

#endif //DEFINED_TCOD_HPP
