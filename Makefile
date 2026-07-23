CFLAGS = -Wall -g -O0 -std=c99 $(shell pkg-config --cflags sdl3)
LDLIBS = $(shell pkg-config --libs sdl3) -lm

build:
	gcc $(CFLAGS) ./src/*.c -o ./out/game $(LDLIBS)
run:
	./out/game
clean:
	rm ./out/game