#include <SDL3/SDL.h>
#include <SDL3/SDL_error.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>
#include <stdio.h>

#include "enginie/core/config.h"
#include "enginie/platform/platform.h"

int is_game_running = false;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

int initialize_sdl(void)
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "Couldn't initialize SDL: %s", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow(NULL, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    if (!window)
    {
        fprintf(stderr, "Couldn't init window, %s", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, NULL);

    if (!renderer)
    {
        fprintf(stderr, "Couldn't create renderer: %s\n", SDL_GetError());
        return false;
    }

    SDL_SetRenderVSync(renderer, 1);
    return true;
}

void process_input(void)
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_EVENT_QUIT:
            is_game_running = false;
            break;

        case SDL_EVENT_KEY_DOWN:
            if (!event.key.repeat && event.key.key == SDLK_ESCAPE)
                is_game_running = false;
            break;
        }
    }
}

void destroy_window(void)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
