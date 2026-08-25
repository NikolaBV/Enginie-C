#include "enginie/systems/collision.h"
#include <enginie/ecs/components.h>
#include <enginie/ecs/entity.h>
#include <stdio.h>

bool aabb(float first_entity_x, float first_entity_y, float second_entity_x, float second_entity_y,
          int first_entity_width, int first_entity_height, int second_entity_width, int second_entity_height)
{
    if (first_entity_x < second_entity_x + second_entity_width &&
        first_entity_x + first_entity_width > second_entity_x &&
        first_entity_y < second_entity_y + second_entity_height &&
        first_entity_y + first_entity_height > second_entity_y)
    {
        return true;
    }
    return false;
}

void update_collision_system(int entity_id)
{
    // TODO the system is not generic yet, this was build just to test if it works
    PositionComponent *first_entity_position = &components->position_components[entity_id];
    SpriteComponent *first_entity_sprite = &components->sprite_components[entity_id];
    VelocityComponent *first_entity_velocity = &components->velocity_components[entity_id];

    for (int second_entity_id = 1; second_entity_id < number_of_entities; ++second_entity_id)
    {
        if (entity_id == second_entity_id)
        {
            continue;
        }
        else
        {
            PositionComponent *second_entity_position = &components->position_components[second_entity_id];
            SpriteComponent *second_entity_sprite = &components->sprite_components[second_entity_id];

            if (aabb(first_entity_position->x, first_entity_position->y, second_entity_position->x, second_entity_position->y,
                     first_entity_sprite->sprite_width, first_entity_sprite->sprite_height, second_entity_sprite->sprite_width, second_entity_sprite->sprite_height))
            {
                first_entity_velocity->x = 0;
                first_entity_velocity->y = 0;
                first_entity_velocity->speed = 0;
            }
        }
    }
}