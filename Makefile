CFLAGS = -Wextra -Wshadow -Wunused-parameter -fsanitize=address,undefined -Wall -g -O0 -std=c99 $(shell pkg-config --cflags sdl3 sdl3-image)
LDLIBS = $(shell pkg-config --libs sdl3 sdl3-image) -lm

build:
	gcc $(CFLAGS) ./src/*.c -o ./out/game $(LDLIBS)
run:
	LSAN_OPTIONS=suppressions=lsan.supp:exitcode=0 ./out/game
clean:
	rm ./out/game