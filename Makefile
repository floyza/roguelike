TARGET = roguelike
OBJS = main.o map.o tcod.o game.o player.o creature.o
CXXFLAGS += -std=c++14 -Wall -I/usr/include/libtcod -ltcod -ltcodxx

ifdef DEBUG
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

main.o: main.cpp game.hpp map.hpp player.hpp creature.hpp tcod.hpp
	g++ $(CXXFLAGS) -c -o $@ $<

map.o: map.cpp map.hpp tcod.hpp player.hpp
	g++ $(CXXFLAGS) -c -o $@ $<

tcod.o: tcod.cpp tcod.hpp
	g++ $(CXXFLAGS) -c -o $@ $<

game.o: game.cpp game.hpp map.hpp player.hpp creature.hpp tcod.hpp
	g++ $(CXXFLAGS) -c -o $@ $<

player.o: player.cpp player.hpp creature.hpp
	g++ $(CXXFLAGS) -c -o $@ $<

creature.o: creature.cpp creature.hpp tcod.hpp
	g++ $(CXXFLAGS) -c -o $@ $<

.PHONY: clean

clean:
	${RM} $(TARGET) $(OBJS)
