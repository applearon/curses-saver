CFLAGS=-Wall -g
LDFLAGS=-lncurses
IS_DEBUG = ${DEBUG_BUILD}
CC=
ifeq ($(OS), Windows_NT)
	CC += x86_64-w64-mingw32-gcc
else
	CC += gcc
endif
ifeq ($(IS_DEBUG), 1)
	CFLAGS += -fsanitize=address
endif

BINARY=curses-saver
OUT=./out
OBJS=$(patsubst src/%.c,out/%.o,$(wildcard src/*.c))
SRC=$(wildcard src/*.c)

dir_guard=@mkdir -p $(@D)

.PHONY: all clean directories

all: directories ./out/$(BINARY)

./out/$(BINARY): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(OUT)/$(BINARY) $(LDFLAGS)

$(OBJS): out/%.o : src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rvf $(OUT)

directories: ${OUT}

${OUT}:
	$(dir_guard) $(OUT)


run:
	./out/$(BINARY) ./examples/arch
