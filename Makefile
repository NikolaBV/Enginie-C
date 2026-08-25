CC  = gcc
OUT = out/game
SRC = $(shell find src -name '*.c')
OBJ = $(SRC:src/%.c=build/%.o)
DEP = $(OBJ:.o=.d)

WARNINGS = -Wall -Wextra -Wshadow -Wstrict-prototypes -Wunused-parameter \
           -pedantic -Wold-style-definition
CFLAGS   = -Iinclude -std=c99 -g -O0 $(WARNINGS) -MMD -MP \
           -fsanitize=address,undefined \
           $(shell pkg-config --cflags sdl3 sdl3-image)
LDLIBS   = $(shell pkg-config --libs sdl3 sdl3-image) -lm

.PHONY: all build run clean

all: build

build: $(OUT)

$(OUT): $(OBJ)
	mkdir -p $(dir $(OUT))
	$(CC) $(CFLAGS) $(OBJ) -o $(OUT) $(LDLIBS)

build/%.o: src/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

run: build
	LSAN_OPTIONS=suppressions=lsan.supp:exitcode=0 ./$(OUT)

clean:
	rm -rf out build

-include $(DEP)
