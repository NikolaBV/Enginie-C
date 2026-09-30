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
#include "enginie/systems/health.h"
#include "enginie/systems/collision.h"
#include "game/content.h"
#include "game/game.h"

#define BUMP_DAMAGE 10
#define TREE_HEAL 15

static void on_collision_enter(int a, int b)
{
    CollisionComponent *collison_a = &components->collision_components[a];
    CollisionComponent *collison_b = &components->collision_components[b];

    HealthComponent *health_a = &components->health_components[a];
    HealthComponent *health_b = &components->health_components[b];

    if ((collison_a->layer & LAYER_PLAYER) && (collison_b->layer & LAYER_PLAYER))
    {
        apply_damage(a, BUMP_DAMAGE);
        apply_damage(b, BUMP_DAMAGE);

        printf("Entity %d health is at: %d\n", a, health_a->health);
        printf("Entity %d health is at: %d\n", b, health_b->health);
        return;
    }

    if ((collison_a->layer & LAYER_PLAYER) && (collison_b->layer & LAYER_WORLD))
    {
        heal(a, TREE_HEAL);
        printf("Entity %d health is at: %d\n", a, health_a->health);
    }
    else if ((collison_b->layer & LAYER_PLAYER) && (collison_a->layer & LAYER_WORLD))
    {
        heal(b, TREE_HEAL);
        printf("Entity %d health is at: %d\n", b, health_b->health);
    }
}

static void resolve_collision_events(void)
{
    for (int i = 0; i < current_collision_count; ++i)
    {
        int a = collisions[i].a;
        int b = collisions[i].b;

        // fire on the frame contact STARTS, not for every frame it continues
        if (!was_overlapping(a, b))
            on_collision_enter(a, b);
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
    add_health_component_to_entity(player_entity_id, 100);
    add_collision_component_to_entity(player_entity_id, 32, 64, 32, 14, false, false, LAYER_PLAYER, (LAYER_PLAYER | LAYER_ENEMY | LAYER_HAZARD | LAYER_PICKUP | LAYER_WORLD));

    int second_player_entity = create_entity();
    if (second_player_entity < 0)
        return 1;

    add_position_component_to_entity(second_player_entity, 200, 200);
    add_sprite_component_to_entity(second_player_entity, player_clips[CLIP_IDLE].texture_id, 32, 32, 3);
    add_keyboard_input_component_to_entity(second_player_entity, arrows_layout);
    add_animation_component_to_entity(second_player_entity);
    add_velocity_component_to_entity(second_player_entity, 150);
    add_facing_component_to_entity(second_player_entity);
    add_health_component_to_entity(second_player_entity, 100);
    add_collision_component_to_entity(second_player_entity, 32, 64, 32, 14, false, false, LAYER_PLAYER, (LAYER_PLAYER | LAYER_ENEMY | LAYER_HAZARD | LAYER_PICKUP | LAYER_WORLD));

    int tree_entity = create_entity();
    if (tree_entity < 0)
        return 1;

    add_position_component_to_entity(tree_entity, 300, 300);
    add_sprite_component_to_entity(tree_entity, texture_id_of_tree, 32, 54, 3);
    add_collision_component_to_entity(tree_entity, 36, 90, 24, 48, true, false, LAYER_WORLD, LAYER_WORLD);

    last_frame_time = SDL_GetTicks();

    return 0;
}
void update(float delta_time)
{
    for (int entity_id = 0; entity_id < number_of_entities; ++entity_id)
    {
        update_input_system(entity_id);
        update_animation_selection_system(entity_id);
    }
    collision_begin_frame();

    for (int entity_id = 0; entity_id < number_of_entities; ++entity_id)
        update_position_system(entity_id, delta_time, AXIS_X);
    update_collision_system(AXIS_X);

    for (int entity_id = 0; entity_id < number_of_entities; ++entity_id)
        update_position_system(entity_id, delta_time, AXIS_Y);
    update_collision_system(AXIS_Y);
    resolve_collision_events();

    for (int entity_id = 0; entity_id < number_of_entities; ++entity_id)
    {
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
    for (int entity_id = 0; entity_id < number_of_entities; ++entity_id)
    {
        render_collider_debug(entity_id, renderer);
    }
    for (int entity_id = 0; entity_id < number_of_entities; ++entity_id)
    {
        render_health_bar(entity_id, renderer);
    }

    SDL_RenderPresent(renderer);
}
