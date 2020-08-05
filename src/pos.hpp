#ifndef DEFINED_POS_HPP
#define DEFINED_POS_HPP

#include <cmath>

struct Pos {
  Pos() = default;
  constexpr Pos(int x, int y) : x{x}, y{y} {}
  int x;
  int y;

  constexpr Pos &operator+=(const Pos &p) {
    x += p.x;
    y += p.y;
    return *this;
  }
  constexpr Pos &operator-=(const Pos &p) {
    x -= p.x;
    y -= p.y;
    return *this;
  }
};

constexpr inline bool operator==(const Pos &p0, const Pos &p1) {
  return p0.x==p1.x && p0.y==p1.y;
}

constexpr inline Pos operator+(Pos p0, const Pos &p1) {
  return p0 += p1;
}

constexpr inline Pos operator-(Pos p0, const Pos &p1) {
  return p0 += p1;
}

inline int distance(Pos p0, Pos p1) {
  using std::abs;
  int dx = abs(p0.x - p1.x);
  int dy = abs(p0.y - p1.y);
  return std::sqrt(dx * dx + dy * dy);
}

#endif
