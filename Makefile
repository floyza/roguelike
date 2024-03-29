SRC_DIR=src
OBJ_DIR=obj

TARGET = roguelike
_OBJS = main.o map.o tcod_util.o game.o player.o creature.o gui.o monster.o lua.o item.o effect.o status.o command.o input_handler.o factions.o bordered_window.o inventory_gui.o lua_gui.o
OBJS = $(patsubst %,$(OBJ_DIR)/%,$(_OBJS))
CXXFLAGS += -std=c++17 -Wall -Wextra -isystem . -ltcod -llua -MMD -DSOL_ALL_SAFETIES_ON -DSOL_PRINT_ERRORS
DEPENDS = $(OBJS:.o=.d)

ifndef RELEASE
CXXFLAGS += -ggdb -O0
else
ifdef DEBUG
CXXFLAGS += -ggdb
endif
CXXFLAGS += -O2 -DNDEBUG
endif


all: init $(TARGET)

init:
	mkdir -p obj

$(TARGET): $(OBJS)
	g++ $(CXXFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	g++ $(CXXFLAGS) -c -o $@ $<

-include $(OBJ_DIR)/*.d

.PHONY: clean

clean:
	${RM} $(TARGET) $(OBJS) $(DEPENDS)
