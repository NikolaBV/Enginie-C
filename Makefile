CC  = gcc
OUT = out/game
SRC = ./src/*.c

WARNINGS = -Wall -Wextra -Wshadow -Wstrict-prototypes -Wunused-parameter \
           -pedantic -Wold-style-definition
CFLAGS   = -Iinclude -std=c99 -g -O0 $(WARNINGS) -fsanitize=address,undefined \
           $(shell pkg-config --cflags sdl3 sdl3-image)
LDLIBS   = $(shell pkg-config --libs sdl3 sdl3-image) -lm

.PHONY: all build run clean

all: build

build:
	mkdir -p $(dir $(OUT))
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LDLIBS)

run: build
	LSAN_OPTIONS=suppressions=lsan.supp:exitcode=0 ./$(OUT)

clean:
	rm -rf out
