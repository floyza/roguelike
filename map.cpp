#include "map.hpp"
#include "tcod.hpp"
#include "game.hpp"
#include <iostream>
#include <vector>

std::pair<int,int> rand_internal_pt(int x1, int y1, int x2, int y2)
{
  return {x1+(x2-x1)/2, y1+(y2-y1)/2};
}

struct Rect {
  int x1,y1,x2,y2;
};

class Make_room_callback : public ITCODBspCallback {
public:
  std::vector<Rect> node_rooms;
private:
  bool visitNode(TCODBsp *node, void *userData) {
    TCODMap &map=*static_cast<TCODMap*>(userData);
    int x_min=node->x, x_max=node->x+node->w, width=node->w;
    int y_min=node->y, y_max=node->y+node->h, height=node->h;
    Rect room;
    room.x1 = rand_int(x_min, x_min+width/5); 
    room.y1 = rand_int(y_min, y_min+height/5);
    room.x2 = rand_int(x_max-width/5, x_max); 
    room.y2 = rand_int(y_max-height/5, y_max);
    
    for (int x_off=1; x_off < room.x2-room.x1-1; ++x_off) {
      for (int y_off=1; y_off < room.y2-room.y1-1; ++y_off) {
	map.setProperties(room.x1 + x_off, room.y1 + y_off, true, true);
      }
    }

    if (node_rooms.size() > 0) {
      Rect sib_room = node_rooms.back();

      TCODMap omap(map.getWidth(), map.getHeight());
      omap.clear(true, true);
      TCODPath path(&omap);
      int ox, oy, dx, dy;
      auto origin = rand_internal_pt(room.x1, room.y1, room.x2, room.y2);
      ox = origin.first;
      oy = origin.second;
      auto dest = rand_internal_pt(sib_room.x1, sib_room.y1, sib_room.x2, sib_room.y2);
      dx = dest.first;
      dy = dest.second;
      path.compute(ox, oy, dx, dy);
      int x,y;
      while (path.walk(&x,&y, false)) {
	map.setProperties(x, y, true, true);
      }
    }
    node_rooms.push_back(room);
    if (!node->isLeaf())
      return false;
    return true;
  }
};

Map::Map(int w, int h, int depth)
  : map(w,h), depth(depth)
{
  TCODBsp *bsp = new TCODBsp(0,0,w,h);
  bsp->splitRecursive(tcod_rand.get(),4,8,8,1.f,1.f);
  Make_room_callback callback;
  bsp->traverseInvertedLevelOrder(&callback, static_cast<void*>(&map));
}

Drawable Map::get_drawable()
{
  int width = map.getWidth(), height = map.getHeight();
  Drawable d{0,0,Drawable::draw_map{boost::extents[width][height]}};
  for (int x=0; x<width; ++x) {
    for (int y=0; y<height; ++y) {
      char &tile = d.map[x][y];
      if (map.isWalkable(x,y))
	tile = ' ';
      else
	tile = '#';
    }
  }
  return d;
}
