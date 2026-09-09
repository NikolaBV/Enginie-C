#include "enginie/systems/collision.h"
#include <enginie/ecs/components.h>
#include <enginie/ecs/entity.h>

#include <SDL3/SDL.h>
#include <stdio.h>
#include <math.h>

int collision_count = 0;
CollisionPair collisions[MAX_COLLISIONS];

Bounds collider_bounds(int entity_id)
{
    CollisionComponent *collider = &components->collision_components[entity_id];
    PositionComponent *position = &components->position_components[entity_id];

    Bounds bounds;
    bounds.left = position->x + collider->offset_x;
    bounds.right = bounds.left + collider->width;
    bounds.top = position->y + collider->offset_y;
    bounds.bottom = bounds.top + collider->height;

    return bounds;
}

bool bounds_overlap(Bounds a, Bounds b)
{
    return a.left < b.right && a.right > b.left &&
           a.top < b.bottom && a.bottom > b.top;
}
bool layers_should_test(int entity_a_id, int entity_b_id)
{
    CollisionComponent *a = &components->collision_components[entity_a_id];
    CollisionComponent *b = &components->collision_components[entity_b_id];

    return (a->layer & b->mask) || (b->layer & a->mask);
}

void resolve_overlap(int a, int b)
{
    CollisionComponent *collision_a = &components->collision_components[a];
    CollisionComponent *collision_b = &components->collision_components[b];

    if (collision_a->is_static && collision_b->is_static)
        return;

    Bounds bounds_a = collider_bounds(a);
    Bounds bounds_b = collider_bounds(b);

    float overlap_x = fminf(bounds_a.right, bounds_b.right) - fmaxf(bounds_a.left, bounds_b.left);
    float overlap_y = fminf(bounds_a.bottom, bounds_b.bottom) - fmaxf(bounds_a.top, bounds_b.top);

    float center_a_x = (bounds_a.left + bounds_a.right) * 0.5f;
    float center_b_x = (bounds_b.left + bounds_b.right) * 0.5f;

    float center_a_y = (bounds_a.top + bounds_a.bottom) * 0.5f;
    float center_b_y = (bounds_b.top + bounds_b.bottom) * 0.5f;

    float push_x = 0.0f;
    float push_y = 0.0f;

    if (overlap_x < overlap_y)
        push_x = (center_a_x < center_b_x) ? -overlap_x : overlap_x;
    else
        push_y = (center_a_y < center_b_y) ? -overlap_y : overlap_y;

    float share_a = collision_a->is_static ? 0.0f : (collision_b->is_static ? 1.0f : 0.5f);
    float share_b = 1.0f - share_a;

    components->position_components[a].x += push_x * share_a;
    components->position_components[a].y += push_y * share_a;
    components->position_components[b].x -= push_x * share_b;
    components->position_components[b].y -= push_y * share_b;

    printf("pair %d,%d  push = (%.1f, %.1f)\n", a, b, push_x, push_y);
}
void update_collision_system(void)
{
    collision_count = 0;
    for (int a = 0; a < number_of_entities - 1; a++)
    {
        if (!does_entity_have_component(a, Collision_Component_Signature))
            continue;
        if (!does_entity_have_component(a, Position_Component_Signature))
            continue;

        for (int b = a + 1; b < number_of_entities; b++)
        {
            if (!does_entity_have_component(a, Collision_Component_Signature))
                continue;
            if (!does_entity_have_component(a, Position_Component_Signature))
                continue;
            if (!layers_should_test(a, b))
                continue;
            if (!bounds_overlap(collider_bounds(a), collider_bounds(b)))
                continue;

            if (collision_count < MAX_COLLISIONS)
            {
                collisions[collision_count].a = a;
                collisions[collision_count].b = b;
                collision_count++;
            }
            resolve_overlap(a, b);
        }
    }
}
