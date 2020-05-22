#include "tcod.hpp"

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
