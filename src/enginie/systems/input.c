#include <SDL3/SDL_keyboard.h>
#include <assert.h>
#include <math.h>

#include "enginie/ecs/entity.h"
#include "enginie/systems/input.h"

void update_input_system(int entity_id)
{
    assert(entity_id >= 0 && entity_id < MAX_ENTITIES);

    if (does_entity_have_component(entity_id, Keyboard_Input_Component_Signature) && does_entity_have_component(entity_id, Velocity_Component_Signature))
    {
        VelocityComponent *velocity = &components->velocity_components[entity_id];
        KeyboardInputComponent *keyboard_input = &components->keyboard_input_components[entity_id];

        const bool *keys = SDL_GetKeyboardState(NULL);

        float direction_x = (keys[keyboard_input->movement_direction_keys.right] - keys[keyboard_input->movement_direction_keys.left]);
        float direction_y = (keys[keyboard_input->movement_direction_keys.down] - keys[keyboard_input->movement_direction_keys.up]);

        float length_of_movement_vector = sqrtf(direction_x * direction_x + direction_y * direction_y);
        if (length_of_movement_vector > 0.0001f)
        {
            direction_x /= length_of_movement_vector;
            direction_y /= length_of_movement_vector;
        }

        velocity->x = direction_x * velocity->speed;
        velocity->y = direction_y * velocity->speed;
    }
}
