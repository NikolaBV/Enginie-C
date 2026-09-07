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
    // TODO finish the logic
    Bounds bounds_a = collider_bounds(a);
    Bounds bounds_b = collider_bounds(b);

    CollisionComponent *collider_a = &components->collision_components[a];
    CollisionComponent *collider_b = &components->collision_components[b];

    float start_x = fmaxf(bounds_a.left, bounds_b.left);
    float end_x = fminf(bounds_a.right, bounds_b.right);
    float overlap_x = end_x - start_x;

    float start_y = fmaxf(bounds_a.top, bounds_b.top);
    float end_y = fminf(bounds_a.bottom, bounds_b.bottom);
    float overlap_y = end_y - start_y;
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
