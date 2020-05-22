#ifndef DEFINED_TCOD_HPP
#define DEFINED_TCOD_HPP

#include <memory>
#include <libtcod/libtcod.hpp>

extern std::unique_ptr<TCODRandom> tcod_rand;
int rand_int(int min, int max);
int rand_int_log(int min, int max);

#endif //DEFINED_TCOD_HPP
