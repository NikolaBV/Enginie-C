#include <assert.h>
#include <math.h>

#include "enginie/ecs/entity.h"
#include "enginie/systems/facing.h"

void update_facing_system(int entity_id)
{
    assert(entity_id >= 0 && entity_id < MAX_ENTITIES);
    if (does_entity_have_component(entity_id, Facing_Component_Signature) && does_entity_have_component(entity_id, Velocity_Component_Signature))
    {
        VelocityComponent *velocity = &components->velocity_components[entity_id];
        FacingComponent *facing = &components->facing_components[entity_id];

        if (fabsf(velocity->x) < 0.001f && fabsf(velocity->y) < 0.001f)
            return;

        if (fabsf(velocity->x) >= fabsf(velocity->y))
        {
            facing->facing = FACE_SIDE;
            facing->flip = (velocity->x < 0);
        }
        else
        {
            facing->facing = (velocity->y > 0) ? FACE_DOWN : FACE_UP;
        }
    }
}
