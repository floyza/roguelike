#include "factions.hpp"

bool is_enemies(Faction f1, Faction f2) {
  return f1 == f2; // right now, all factions are enemies to each other
}
