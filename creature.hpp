#ifndef CREATURE_HPP_DEFINED
#define CREATURE_HPP_DEFINED

class Creature {
  char icon;
public:
  Creature(char icon, int x=0, int y=0);
  int x,y;
  void draw();
  virtual ~Creature() {}
};

#endif //CREATURE_HPP_DEFINED
