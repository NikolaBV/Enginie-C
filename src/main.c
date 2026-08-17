#include <stdio.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_error.h>

#include "constants.h"
#include "../lib/ecs.h"

int is_game_running = FALSE;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

Uint64 last_frame_time = 0;
const float FIXED_DT = 1.0f / 60.0f;
float accumulator = 0.0f;

int main(void);
int setup(void);

void destroy_window(void);
void update(float deltaTime);
void render(void);

int initialize_sdl(void)
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "Couldn't initialize SDL: %s", SDL_GetError());
        return FALSE;
    }

    window = SDL_CreateWindow(NULL, WINDOW_WIDTH, WINDOW_HEIGHT, 0);

    if (!window)
    {
        fprintf(stderr, "Couldn't init window, %s", SDL_GetError());
        return FALSE;
    }

    renderer = SDL_CreateRenderer(window, NULL);

    if (!renderer)
    {
        fprintf(stderr, "Couldn't create renderer: %s\n", SDL_GetError());
        return FALSE;
    }

    SDL_SetRenderVSync(renderer, 1);
    return TRUE;
}

void process_input(void)
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_EVENT_QUIT:
            is_game_running = FALSE;
            break;

        case SDL_EVENT_KEY_DOWN:
            if (!event.key.repeat && event.key.key == SDLK_ESCAPE)
                is_game_running = FALSE;
            break;
        }
    }
}

int setup()
{
    uint32_t texture_id_of_player = load_image_as_texture("assets/idle.png", renderer);
    if (texture_id_of_player == UINT32_MAX)
    {
        fprintf(stderr, "Couldn't load texture of player entity \n");
        return 1;
    }
    SDL_Texture *playerTexture = get_texture_by_texture_id(texture_id_of_player);

    if (playerTexture == NULL)
    {
        fprintf(stderr, "Couldn't load texture of player entity \n");
        return 1;
    }

    int player_entity_id = create_entity();

    add_position_component_to_components(player_entity_id, 100, 100);
    add_sprite_component_to_components(player_entity_id, texture_id_of_player, playerTexture->w, playerTexture->h, 32, 32, 2);
    add_keyboard_input_component_to_components(player_entity_id);
    add_animation_component_to_entity(player_entity_id);
    add_velocity_component_to_entity(player_entity_id, 50);

    last_frame_time = SDL_GetTicks();

    return 0;
}
void update(float deltaTime)
{
    for (int entity_id_index = 0; entity_id_index < components->total_position_components; ++entity_id_index)
    {
        if (does_entity_have_component(entity_id_index, POSITION_COMPONENT_SIGNATURE))
        {
            update_position_system(entity_id_index, deltaTime);
        }

        if (does_entity_have_component(entity_id_index, AnimationComponentSignature))
        {
            update_animation_system(entity_id_index, deltaTime);
        }
        if (does_entity_have_component(entity_id_index, KeyboardInputComponentSignature))
        {
            update_input_system(entity_id_index);
        }
    }
}

void render()
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    for (int i = 0; i < components->total_sprite_components; ++i)
    {
        if (does_entity_have_component(i, SPRITE_COMPONENT_SIGNATURE))
        {
            update_render_system(i, renderer);
        }
    }

    SDL_RenderPresent(renderer);
}
void destroy_window()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main()
{
    if (!initialize_sdl())
    {
        return 1;
    }
    is_game_running = TRUE;

    if (setup() == 1)
    {
        return 1;
    }

    while (is_game_running)
    {
        process_input();
        Uint64 currentTicks = SDL_GetTicks();
        float frameTime = (currentTicks - last_frame_time) / 1000.0f;
        last_frame_time = currentTicks;
        if (frameTime > 0.25f)
            frameTime = 0.25f;

        accumulator += frameTime;
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