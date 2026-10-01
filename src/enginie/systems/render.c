#include <assert.h>

#include "enginie/ecs/entity.h"
#include "enginie/platform/texture_store.h"
#include "enginie/systems/render.h"
#include <enginie/systems/collision.h>

void update_render_system(int entity_id, SDL_Renderer *renderer)
{
    assert(entity_id >= 0 && entity_id < MAX_ENTITIES);

    if (does_entity_have_component(entity_id, Sprite_Component_Signature) && does_entity_have_component(entity_id, Position_Component_Signature))
    {
        SpriteComponent *sprite = &components->sprite_components[entity_id];
        PositionComponent *position = &components->position_components[entity_id];

        Facing facing_row = FACE_DOWN;
        bool flip = false;

        if (does_entity_have_component(entity_id, Facing_Component_Signature))
        {
            facing_row = components->facing_components[entity_id].facing;
            flip = components->facing_components[entity_id].flip;
        }

        SDL_Texture *entity_texture = NULL;
        float src_rect_x = 0;

        if (does_entity_have_component(entity_id, Animation_Component_Signature))
        {
            AnimationComponent *animation = &components->animation_components[entity_id];

            src_rect_x = animation->frame_index * sprite->sprite_width;
            entity_texture = get_texture_by_texture_id(animation->clips[animation->active_clip].texture_id);
        }
        else
        {
            entity_texture = get_texture_by_texture_id(sprite->texture_id);
        }

        SDL_FRect src_rect = {src_rect_x, facing_row * sprite->sprite_height, sprite->sprite_width, sprite->sprite_height};
        SDL_FRect dest_rect = {position->x, position->y, sprite->sprite_width * sprite->scale_x, sprite->sprite_height * sprite->scale_y};

        SDL_RenderTextureRotated(renderer, entity_texture, &src_rect, &dest_rect, 0.0, NULL, flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
    }
}

void render_collider_debug(int entity_id, SDL_Renderer *renderer)
{
    if (does_entity_have_component(entity_id, Collision_Component_Signature) && does_entity_have_component(entity_id, Position_Component_Signature))
    {
        Bounds bounds = collider_bounds(entity_id);

        SDL_FRect collision_rect = {bounds.left, bounds.top, bounds.right - bounds.left, bounds.bottom - bounds.top};

        CollisionComponent *collision = &components->collision_components[entity_id];

        if (collision->is_trigger)
        {
            set_render_draw_color(blue, renderer);
        }
        else if (collision->is_static)
        {
            set_render_draw_color(red, renderer);
        }

        else
        {
            set_render_draw_color(magenta, renderer);
        }

        SDL_RenderRect(renderer, &collision_rect);
    }
}

void render_health_bar(int entity_id, SDL_Renderer *renderer)
{
    assert(entity_id >= 0 && entity_id < MAX_ENTITIES);

    if (!does_entity_have_component(entity_id, Health_Component_Signature))
        return;
    if (!does_entity_have_component(entity_id, Position_Component_Signature))
        return;
    if (!does_entity_have_component(entity_id, Sprite_Component_Signature))
        return;

    const PositionComponent *position = &components->position_components[entity_id];
    const SpriteComponent *sprite = &components->sprite_components[entity_id];
    const HealthComponent *health = &components->health_components[entity_id];

    if (health->max_health <= 0)
        return;

    float width = (float)(sprite->sprite_width * sprite->scale_x);
    float fraction = (float)health->health / (float)health->max_health;

    SDL_FRect back = {position->x, position->y - 10.0f, width, 6.0f};
    SDL_FRect front = {position->x, position->y - 10.0f, width * fraction, 6.0f};

    SDL_SetRenderDrawColor(renderer, 60, 20, 30, 255);
    SDL_RenderFillRect(renderer, &back);
    SDL_SetRenderDrawColor(renderer, 80, 220, 120, 255);
    SDL_RenderFillRect(renderer, &front);
}