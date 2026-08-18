#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_error.h>

#include "../lib/ecs.h"

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

    add_position_component_to_entity(player_entity_id, 100, 100);
    add_sprite_component_to_entity(player_entity_id, texture_id_of_player, 32, 32, 3);

    add_keyboard_input_component_to_entity(player_entity_id, wasd_layout);
    add_animation_component_to_entity(player_entity_id);
    add_velocity_component_to_entity(player_entity_id, 150);
    add_facing_component_entity(player_entity_id);

    int second_player_entity = create_entity();

    add_position_component_to_entity(second_player_entity, 200, 200);
    add_sprite_component_to_entity(second_player_entity, texture_id_of_player, 32, 32, 3);
    add_keyboard_input_component_to_entity(second_player_entity, arrows_layout);
    add_animation_component_to_entity(second_player_entity);
    add_velocity_component_to_entity(second_player_entity, 150);
    add_facing_component_entity(second_player_entity);

    last_frame_time = SDL_GetTicks();

    return 0;
}
void update(float delta_time)
{
    for (int entity_id_index = 0; entity_id_index < number_of_entities; ++entity_id_index)
    {
        if (does_entity_have_component(entity_id_index, Keyboard_Input_Component_Signature))
        {
            update_input_system(entity_id_index);
        }
        if (does_entity_have_component(entity_id_index, Position_Component_Signature))
        {
            update_position_system(entity_id_index, delta_time);
        }

        if (does_entity_have_component(entity_id_index, Facing_Component_Signature))
        {
            update_facing_system(entity_id_index);
        }

        if (does_entity_have_component(entity_id_index, Animation_Component_Signature))
        {
            update_animation_system(entity_id_index, delta_time);
        }
    }
}
void render()
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    for (int i = 0; i < number_of_entities; ++i)
    {
        if (does_entity_have_component(i, Sprite_Component_Signature))
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
    is_game_running = true;

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