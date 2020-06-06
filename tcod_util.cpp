#include "tcod_util.hpp"
#include <array>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>

std::unique_ptr<TCODRandom> tcod_rand{};

int rand_int(int min, int max) {
  tcod_rand->setDistribution(TCOD_DISTRIBUTION_LINEAR);
  return tcod_rand->getInt(min, max);
}

int rand_int_log(int min, int max) {
  tcod_rand->setDistribution(TCOD_DISTRIBUTION_GAUSSIAN_RANGE);
  int result=-1;
  while (result < 0)
    result = tcod_rand->getInt(min-max, max-min);
  return min + result;
}

bool percent_chance(double chance) {
  chance/=100;
  int integral = std::floor(chance);
  double decimal = chance-integral;
  constexpr int prec = 100000000;
  int gcd = std::gcd(std::lround(decimal*prec), prec);
  int numerator = std::lround(decimal * prec) / gcd;
  int denominator = prec / gcd;
  return rand_int(1, denominator) <= numerator;
}

struct Wall_tile {
  bool n,s,w,e;
  bool operator==(const Wall_tile &o) const {return n==o.n && s==o.s && e==o.e && w==o.w;}
  void operator+=(const Wall_tile &o) {n+=o.n; s+=o.s; e+=o.e; w+=o.w;}
};

int wall_merge(int wall1, int wall2) {
  std::array<int, 11> wall_chars
    {
     TCOD_CHAR_HLINE,
     TCOD_CHAR_VLINE,
     TCOD_CHAR_NE,
     TCOD_CHAR_NW,
     TCOD_CHAR_SE,
     TCOD_CHAR_SW,
     TCOD_CHAR_TEEW,
     TCOD_CHAR_TEEE,
     TCOD_CHAR_TEEN,
     TCOD_CHAR_TEES,
     TCOD_CHAR_CROSS,
    };
  std::array<Wall_tile, 11> wall_data
    {
     Wall_tile{0,0,1,1},
     Wall_tile{1,1,0,0},
     Wall_tile{0,1,1,0},
     Wall_tile{0,1,0,1},
     Wall_tile{1,0,1,0},
     Wall_tile{1,0,0,1},
     Wall_tile{1,1,1,0},
     Wall_tile{1,1,0,1},
     Wall_tile{1,0,1,1},
     Wall_tile{0,1,1,1},
     Wall_tile{1,1,1,1}
    };
  std::ptrdiff_t wall1_it = std::find(wall_chars.begin(), wall_chars.end(), wall1) - wall_chars.begin();
  std::ptrdiff_t wall2_it = std::find(wall_chars.begin(), wall_chars.end(), wall2) - wall_chars.begin();
  // using std::size_t;
  // if (static_cast<size_t>(wall1_it) == wall_chars.size() || static_cast<size_t>(wall2_it) == wall_chars.size())
  //   throw std::runtime_error{"wall_merge: invalid argument"};
  Wall_tile result = wall_data[wall1_it];
  result += wall_data[wall2_it];
  return wall_chars[std::find(wall_data.data(), wall_data.end(), result) - wall_data.begin()];
}
