#pragma once

#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

extern int is_game_running;
extern SDL_Window *window;
extern SDL_Renderer *renderer;

int initialize_sdl(void);
void destroy_window(void);
void process_input(void);
