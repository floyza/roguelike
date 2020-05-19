#include "tcod.hpp"

std::unique_ptr<TCODRandom> tcod_rand{};
int rand_int(int min, int max) {
  tcod_rand->setDistribution(TCOD_DISTRIBUTION_LINEAR);
  return tcod_rand->getInt(min, max);
}
