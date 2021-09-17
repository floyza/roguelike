#ifndef __JSON_DEFINITIONS_H_
#define __JSON_DEFINITIONS_H_

#include "nlohmann/json.hpp"
#include "tcod_util.hpp"

namespace nlohmann {
template <> struct adl_serializer<TCODColor> {
  static void to_json(json &j, const TCODColor &color) {
    j["r"] = color.r;
    j["g"] = color.g;
    j["b"] = color.b;
  }
  static void from_json(const json &j, TCODColor &color) {
    color.r = j.at("r").get<uint8_t>();
    color.g = j.at("g").get<uint8_t>();
    color.b = j.at("b").get<uint8_t>();
  }
};
} // namespace nlohmann

// NLOHMAN_DEFINE_TYPE_NON_INTRUSIVE(TCODColor, r, g, b)

#endif // __JSON_DEFINITIONS_H_
