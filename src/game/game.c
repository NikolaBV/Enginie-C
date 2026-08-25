#include <SDL3/SDL.h>
#include <stdio.h>

#include "enginie/core/config.h"
#include "enginie/core/time.h"
#include "enginie/ecs/entity.h"
#include "enginie/platform/platform.h"
#include "enginie/platform/texture_store.h"
#include "enginie/systems/animation.h"
#include "enginie/systems/facing.h"
#include "enginie/systems/input.h"
#include "enginie/systems/movement.h"
#include "enginie/systems/render.h"
#include "enginie/systems/collision.h"
#include "enginie/systems/health.h"
#include "game/content.h"
#include "game/game.h"

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
    add_collision_component_to_entity(player_entity_id);
    add_health_component_to_entity(player_entity_id, 100);

    int second_player_entity = create_entity();
    if (second_player_entity < 0)
        return 1;

    add_position_component_to_entity(second_player_entity, 200, 200);
    add_sprite_component_to_entity(second_player_entity, player_clips[CLIP_IDLE].texture_id, 32, 32, 3);
    add_keyboard_input_component_to_entity(second_player_entity, arrows_layout);
    add_animation_component_to_entity(second_player_entity);
    add_velocity_component_to_entity(second_player_entity, 150);
    add_facing_component_to_entity(second_player_entity);
    add_health_component_to_entity(second_player_entity, 2000);

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
        update_collision_system(entity_id);
        update_health_system(entity_id);
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
