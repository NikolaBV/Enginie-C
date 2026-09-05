#include "enginie/systems/collision.h"
#include <enginie/ecs/components.h>
#include <enginie/ecs/entity.h>
#include <stdio.h>

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
void update_collision_system(void)
{
    collision_count = 0;
    for (int a = 0; a < number_of_entities - 1; a++)
    {
        if (!does_entity_have_component(a, Collision_Component_Signature))
            continue;
        if (!does_entity_have_component(a, Position_Component_Signature))
            continue;

        for (int b = a + 1; b < number_of_entities - 1; b++)
        {
            if (!does_entity_have_component(a, Collision_Component_Signature))
                continue;
            if (!does_entity_have_component(a, Position_Component_Signature))
                continue;
            if (!bounds_overlap(collider_bounds(a), collider_bounds(b)))
                continue;

            if (collision_count < MAX_COLLISIONS)
            {
                collisions[collision_count].a = a;
                collisions[collision_count].b = b;
                collision_count++;
            }
            printf("overlap: %d and %d\n", a, b);
        }
    }
}