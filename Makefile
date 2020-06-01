TARGET = roguelike
OBJS = main.o map.o tcod_util.o game.o player.o creature.o gui.o monster.o
CXXFLAGS += -std=c++17 -Wall -I/usr/include/libtcod -ltcod -ltcodxx

ifndef RELEASE
CXXFLAGS += -ggdb -O0
else
CXXFLAGS += -O2
endif

ifdef PROF
CXXFLAGS += -g -pg
endif


all: $(TARGET)

$(TARGET): $(OBJS)
	g++ $(CXXFLAGS) -o $@ $^

main.o: main.cpp game.hpp map.hpp player.hpp creature.hpp tcod_util.hpp gui.hpp
	g++ $(CXXFLAGS) -c -o $@ $<

map.o: map.cpp map.hpp tcod_util.hpp creature.hpp player.hpp monster.hpp game.hpp
	g++ $(CXXFLAGS) -c -o $@ $<

tcod.o: tcod_util.cpp tcod_util.hpp
	g++ $(CXXFLAGS) -c -o $@ $<

game.o: game.cpp game.hpp map.hpp player.hpp creature.hpp gui.hpp monster.hpp
	g++ $(CXXFLAGS) -c -o $@ $<

player.o: player.cpp player.hpp creature.hpp game.hpp map.hpp
	g++ $(CXXFLAGS) -c -o $@ $<

monster.o: monster.cpp monster.hpp creature.hpp map.hpp game.hpp player.hpp
	g++ $(CXXFLAGS) -c -o $@ $<

creature.o: creature.cpp creature.hpp
	g++ $(CXXFLAGS) -c -o $@ $<

gui.o: gui.cpp gui.hpp tcod_util.hpp
	g++ $(CXXFLAGS) -c -o $@ $<

.PHONY: clean

clean:
	${RM} $(TARGET) $(OBJS)
