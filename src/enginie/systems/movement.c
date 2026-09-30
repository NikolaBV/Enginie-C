#include <assert.h>

#include "enginie/ecs/entity.h"
#include "enginie/systems/movement.h"

void update_position_system(int entity_id, float delta_time, Axis axis)
{
    assert(entity_id >= 0 && entity_id < MAX_ENTITIES);

    if (does_entity_have_component(entity_id, Position_Component_Signature) && does_entity_have_component(entity_id, Velocity_Component_Signature))
    {
        PositionComponent *position = &components->position_components[entity_id];
        VelocityComponent *velocity = &components->velocity_components[entity_id];

        if (axis == AXIS_X)
            position->x += velocity->x * delta_time;
        else
            position->y += velocity->y * delta_time;
    }
}
