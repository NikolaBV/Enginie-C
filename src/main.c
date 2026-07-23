#include <stdio.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_error.h>

#include "constants.h"

int is_game_running = FALSE;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
Uint64 last_frame_time = 0;

int player_entity_id = 0;
int ENTITIES = 0;
ComponentLists components;

const float FIXED_DT = 1.0f / 60.0f;
float accumulator = 0.0f;

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

void create_entity(float x, float y, uint32_t texture_width, uint32_t texture_height, uint32_t sprite_width, uint32_t sprite_height, ComponentLists *components)
{
    int id = ENTITIES++;
    components->position_components[id].entity_id = id;

    components->position_components[id].x = x;
    components->position_components[id].y = y;
    components->total_position_components++;

    components->sprite_components[id].entity_id = id;
    components->sprite_components[id].sprite_height = sprite_height;
    components->sprite_components[id].sprite_width = sprite_width;
    components->sprite_components[id].texture_width = texture_width;
    components->sprite_components[id].texture_height = texture_height;

    components->total_sprite_components++;

    components->keyboard_input_components[id].entity_id = id;
    components->total_keyboard_input_components++;
}

void update_position_system(PositionComponent *position, KeyboardInput *keyboardInput, ComponentLists *components, float deltaTime)
{
    if (keyboardInput->up)
    {
        position->y -= MOVEMENT_SPEED_IN_PIXELS * deltaTime;
    }
    if (keyboardInput->down)
    {
        position->y += MOVEMENT_SPEED_IN_PIXELS * deltaTime;
    }
    if (keyboardInput->right)
    {
        position->x += MOVEMENT_SPEED_IN_PIXELS * deltaTime;
    }
    if (keyboardInput->left)
    {
        position->x -= MOVEMENT_SPEED_IN_PIXELS * deltaTime;
    }
}
void update_render_system(SpriteComponent *sprite, PositionComponent *position, ComponentLists *components)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_FRect entity_rect = {position->x, position->y, sprite->sprite_width, sprite->sprite_height};
    SDL_RenderFillRect(renderer, &entity_rect);
}

void process_input(ComponentLists *components)
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
            if (event.key.repeat == 0)
            {
                switch (event.key.key)
                {
                case SDLK_W:
                    printf("Pressed W\n");
                    components->keyboard_input_components[player_entity_id].up = true;
                    break;
                case SDLK_S:
                    components->keyboard_input_components[player_entity_id].down = true;

                    break;
                case SDLK_A:
                    components->keyboard_input_components[player_entity_id].left = true;

                    break;
                case SDLK_D:
                    components->keyboard_input_components[player_entity_id].right = true;
                    break;

                case SDLK_ESCAPE:
                    is_game_running = false;
                    break;
                }
            }
            break;

        case SDL_EVENT_KEY_UP:
            if (event.key.repeat == 0)
            {
                switch (event.key.key)
                {
                case SDLK_W:
                    components->keyboard_input_components[player_entity_id].up = false;
                    break;
                case SDLK_S:
                    components->keyboard_input_components[player_entity_id].down = false;
                    break;
                case SDLK_A:
                    components->keyboard_input_components[player_entity_id].left = false;
                    break;
                case SDLK_D:
                    components->keyboard_input_components[player_entity_id].right = false;
                    break;
                }
            }
            break;
        }
    }
}

void setup()
{
    player_entity_id = ENTITIES;
    create_entity(50, 50, 20, 20, 20, 20, &components);
    printf("Player entity id: %d\n", player_entity_id);

    create_entity(120, 100, 20, 20, 20, 20, &components);
    printf("Not the entity id: %d\n", ENTITIES);

    last_frame_time = SDL_GetTicks();
}
void update(float deltaTime)
{
    for (int i = 0; i < components.total_position_components; ++i)
    {
        update_position_system(&components.position_components[i], &components.keyboard_input_components[i], &components, deltaTime);
    }
}
void render()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for (int i = 0; i < components.total_sprite_components; ++i)
    {
        update_render_system(&components.sprite_components[i], &components.position_components[i], &components);
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
    is_game_running = initialize_sdl();

    setup();

    while (is_game_running)
    {
        process_input(&components);
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