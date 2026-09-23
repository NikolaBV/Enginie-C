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

void resolve_overlap(int a, int b, Axis axis)
{
    const CollisionComponent *ca = &components->collision_components[a];
    const CollisionComponent *cb = &components->collision_components[b];

    if (ca->is_static && cb->is_static) // two trees: nothing to separate, ever
        return;

    Bounds ba = collider_bounds(a);
    Bounds bb = collider_bounds(b);

    // only this axis has moved since the last resolve, so only this axis is undone
    float overlap, center_a, center_b;
    if (axis == AXIS_X)
    {
        overlap = fminf(ba.right, bb.right) - fmaxf(ba.left, bb.left);
        center_a = (ba.left + ba.right) * 0.5f;
        center_b = (bb.left + bb.right) * 0.5f;
    }
    else
    {
        overlap = fminf(ba.bottom, bb.bottom) - fmaxf(ba.top, bb.top);
        center_a = (ba.top + ba.bottom) * 0.5f;
        center_b = (bb.top + bb.bottom) * 0.5f;
    }

    float push = (center_a < center_b) ? -overlap : overlap;

    // a static body absorbs none of the correction; two dynamic bodies split it
    float share_a = ca->is_static ? 0.0f : (cb->is_static ? 1.0f : 0.5f);
    float share_b = 1.0f - share_a;

    if (axis == AXIS_X)
    {
        components->position_components[a].x += push * share_a;
        components->position_components[b].x -= push * share_b;
    }
    else
    {
        components->position_components[a].y += push * share_a;
        components->position_components[b].y -= push * share_b;
    }
}
void update_collision_system(Axis axis)
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

            CollisionComponent *collision_a = &components->collision_components[a];
            CollisionComponent *collision_b = &components->collision_components[b];

            bool trigger = collision_a->is_trigger || collision_b->is_trigger;

            if (collision_count < MAX_COLLISIONS)
            {
                collisions[collision_count].a = a;
                collisions[collision_count].b = b;
                collisions[collision_count].trigger = trigger;
                collision_count++;
            }

            if (!trigger)
                resolve_overlap(a, b, axis);
        }
    }
}
