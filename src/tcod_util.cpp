#include "tcod_util.hpp"
#include <array>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <cassert>

std::unique_ptr<TCODRandom> tcod_rand{};

int rand_int(int min, int max) {
  tcod_rand->setDistribution(TCOD_DISTRIBUTION_LINEAR);
  return tcod_rand->getInt(min, max);
}

int rand_int_log(int min, int max) {
  tcod_rand->setDistribution(TCOD_DISTRIBUTION_GAUSSIAN_RANGE);
  return tcod_rand->getInt(min, max);
}

int rand_int_log_bounded(int min, int max, int bmin, int bmax) {
  while (true) {
    int result = rand_int_log(min,max);
    if (bmin <= result && result <= bmax)
      return result;
  }
}

bool percent_chance(double chance) {
  chance/=100;
  constexpr int prec = 100000000;
  int gcd = std::gcd(std::lround(chance*prec), prec);
  int numerator = std::lround(chance * prec) / gcd;
  int denominator = prec / gcd;
  return rand_int(1, denominator) <= numerator;
}

struct Wall_tile {
  bool n,s,w,e;
  bool operator==(const Wall_tile &o) const {return n==o.n && s==o.s && e==o.e && w==o.w;}
  Wall_tile &operator+=(const Wall_tile &o) {n+=o.n; s+=o.s; e+=o.e; w+=o.w; return *this;}
};

int wall_merge(int wall1, int wall2) {
  std::array<int, 12> wall_chars {
		' ',
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
  std::array<Wall_tile, 12> wall_data {
		Wall_tile{0, 0, 0, 0},
		Wall_tile{0, 0, 1, 1},
		Wall_tile{1, 1, 0, 0},
		Wall_tile{0, 1, 1, 0},
		Wall_tile{0, 1, 0, 1},
		Wall_tile{1, 0, 1, 0},
		Wall_tile{1, 0, 0, 1},
		Wall_tile{1, 1, 1, 0},
		Wall_tile{1, 1, 0, 1},
		Wall_tile{1, 0, 1, 1},
		Wall_tile{0, 1, 1, 1},
		Wall_tile{1, 1, 1, 1}
	};
  auto wall1_it = std::find(wall_chars.begin(), wall_chars.end(), wall1);
  auto wall2_it = std::find(wall_chars.begin(), wall_chars.end(), wall2);
  assert(wall1_it != wall_chars.end() || wall2_it != wall_chars.end()); // why did you even call it?
  if (wall1_it == wall_chars.end())
    return wall2;
  if (wall2_it == wall_chars.end())
    return wall1;
  Wall_tile result = wall_data[wall1_it - wall_chars.begin()];
  result += wall_data[wall2_it - wall_chars.begin()];
  return wall_chars[std::find(wall_data.data(), wall_data.end(), result) - wall_data.begin()];
}
