#include <SDL3/SDL.h>
#include <stdio.h>

#include "enginie/core/time.h"
#include "enginie/platform/platform.h"
#include "game/game.h"

int main(void)
{
    if (!initialize_sdl())
    {
        return 1;
    }
    is_game_running = true;

    if (setup() != 0)
    {
        return 1;
    }

    while (is_game_running)
    {
        process_input();
        Uint64 current_ticks = SDL_GetTicks();
        float frame_time = (current_ticks - last_frame_time) / 1000.0f;
        last_frame_time = current_ticks;
        if (frame_time > 0.25f)
            frame_time = 0.25f;

        accumulator += frame_time;
        while (accumulator >= FIXED_DT)
        {
            update(FIXED_DT);
            accumulator -= FIXED_DT;
        }
        render();
    }

    destroy_window();
    return 0;
}
