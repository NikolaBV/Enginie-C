#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_error.h>

#include <stdio.h>

#include "ecs.h"

int is_game_running = false;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

Uint64 last_frame_time = 0;
const float FIXED_DT = 1.0f / 60.0f;
float accumulator = 0.0f;

int main(void);
int setup(void);

void destroy_window(void);
void update(float delta_time);
void render(void);

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

int setup(void)
{
    if (!load_player_clips(renderer))
        return 1;

    uint32_t texture_id_of_tree = load_texture_or_fail(assets_paths[PLANTS], renderer);
    if (texture_id_of_tree == UINT32_MAX)
        return 1;

    int player_entity_id = create_entity();
    if (player_entity_id < 0)
        return 1;

    add_position_component_to_entity(player_entity_id, 100, 100);
    add_sprite_component_to_entity(player_entity_id, player_clips[CLIP_IDLE].texture_id, 32, 32, 3);
    add_keyboard_input_component_to_entity(player_entity_id, wasd_layout);
    add_animation_component_to_entity(player_entity_id);
    add_velocity_component_to_entity(player_entity_id, 150);
    add_facing_component_to_entity(player_entity_id);

    int second_player_entity = create_entity();
    if (second_player_entity < 0)
        return 1;

    add_position_component_to_entity(second_player_entity, 200, 200);
    add_sprite_component_to_entity(second_player_entity, player_clips[CLIP_IDLE].texture_id, 32, 32, 3);
    add_keyboard_input_component_to_entity(second_player_entity, arrows_layout);
    add_animation_component_to_entity(second_player_entity);
    add_velocity_component_to_entity(second_player_entity, 150);
    add_facing_component_to_entity(second_player_entity);

    int tree_entity = create_entity();
    if (tree_entity < 0)
        return 1;

    add_position_component_to_entity(tree_entity, 300, 300);
    add_sprite_component_to_entity(tree_entity, texture_id_of_tree, 32, 54, 3);

    last_frame_time = SDL_GetTicks();

    return 0;
}
void update(float delta_time)
{
    for (int entity_id = 0; entity_id < number_of_entities; ++entity_id)
    {

        update_input_system(entity_id);
        update_animation_selection_system(entity_id);
        update_position_system(entity_id, delta_time);
        update_facing_system(entity_id);
        update_animation_system(entity_id, delta_time);
    }
}
void render(void)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    for (int entity_id = 0; entity_id < number_of_entities; ++entity_id)
    {
        update_render_system(entity_id, renderer);
    }

    SDL_RenderPresent(renderer);
}
void destroy_window(void)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

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