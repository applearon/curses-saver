CFLAGS=-Wall -g
LDFLAGS=-lncurses
IS_DEBUG = ${DEBUG_BUILD}
CC=
ifeq ($(OS), Windows_NT)
	CC += x86_64-w64-mingw32-g++
	CFLAGS += -static -static-libgcc -static-libstdc++
else
	CC += g++
endif
ifeq ($(IS_DEBUG), 1)
	CFLAGS += -fsanitize=address
endif
CC += -std=c++11
BINARY=curses-saver
OUT=./out
OBJS=$(patsubst src/%.cpp,out/%.o,$(wildcard src/*.cpp))
SRC=$(wildcard src/*.cpp)

dir_guard=@mkdir -p $(@D)

.PHONY: all clean directories

all: directories ./out/$(BINARY)

./out/$(BINARY): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(OUT)/$(BINARY) $(LDFLAGS)

$(OBJS): out/%.o : src/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rvf $(OUT)

directories: ${OUT}

${OUT}:
	$(dir_guard) $(OUT)
