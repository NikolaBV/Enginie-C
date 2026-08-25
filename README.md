## Enginie C

This is a second attempt to create an actual game engine that I will use to develop some hobby games. My first attempt was in C++ using syntax and logic I
didn't understand properly, and also I found over time that C++ is quite ugly and overcomplicated at times, so here we are

## Building and running the project

To build the project you need SDL3 installed on your machine, look at the SDL3 docs for your specific distro, after that 
```bash
make
```

And to run:
```bash
make run
```

## Project structure

```
enginie/core/      config, timing
enginie/platform/  SDL window, input polling, texture cache
enginie/ecs/       component structs, entity creation, component storage
enginie/systems/   one file per system (input, movement, facing, animation, render)
game/              content and setup: entities, sprites, clips, key layouts
```
