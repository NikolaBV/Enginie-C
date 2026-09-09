#include <assert.h>
#include <stdio.h>

#include "enginie/ecs/entity.h"
#include "game/content.h"

int number_of_entities = 0;
int signatures[MAX_ENTITIES];

static ComponentLists world_storage;
ComponentLists *components = &world_storage;

int create_entity(void)
{
    if (number_of_entities >= MAX_ENTITIES)
    {
        fprintf(stderr, "Entity limit reached (%d)\n", MAX_ENTITIES);
        return -1;
    }
    return number_of_entities++;
}

void add_component_signature_to_entity(int entity_id, ComponentSignatures component_signature)
{
    assert(entity_id >= 0 && entity_id < MAX_ENTITIES);
    signatures[entity_id] |= component_signature;
}
bool does_entity_have_component(int entity_id, ComponentSignatures component_signature)
{
    assert(entity_id >= 0 && entity_id < MAX_ENTITIES);
    return signatures[entity_id] & component_signature;
}

void add_position_component_to_entity(int entity_id, float x, float y)
{
    assert(entity_id >= 0 && entity_id < MAX_ENTITIES);
    components->position_components[entity_id].x = x;
    components->position_components[entity_id].y = y;

    add_component_signature_to_entity(entity_id, Position_Component_Signature);
}
void add_sprite_component_to_entity(int entity_id, uint32_t texture_id, uint32_t sprite_width, uint32_t sprite_height, int scale)
{
    assert(entity_id >= 0 && entity_id < MAX_ENTITIES);
    components->sprite_components[entity_id].sprite_height = sprite_height;
    components->sprite_components[entity_id].sprite_width = sprite_width;
    components->sprite_components[entity_id].texture_id = texture_id;

    // default the scaler to a value that keeps the sprite unchanged if a number 0 or under is passed
    if (scale <= 0)
    {
        scale = 1;
    }
    components->sprite_components[entity_id].scale = scale;

    add_component_signature_to_entity(entity_id, Sprite_Component_Signature);
}

void add_animation_component_to_entity(int entity_id)
{
    assert(entity_id >= 0 && entity_id < MAX_ENTITIES);
    components->animation_components[entity_id].clips = player_clips;
    components->animation_components[entity_id].active_clip = 0;
    components->animation_components[entity_id].frame_index = 0;
    components->animation_components[entity_id].time_in_frame = 0;
    components->animation_components[entity_id].finished = false;
    components->animation_components[entity_id].clip_count = player_clip_count;

    add_component_signature_to_entity(entity_id, Animation_Component_Signature);
}
void add_facing_component_to_entity(int entity_id)
{
    assert(entity_id >= 0 && entity_id < MAX_ENTITIES);
    components->facing_components[entity_id].facing = FACE_DOWN;
    components->facing_components[entity_id].flip = false;
    add_component_signature_to_entity(entity_id, Facing_Component_Signature);
}

void add_keyboard_input_component_to_entity(int entity_id, Movement_Direction_Keys movement_direction_keys)
{
    assert(entity_id >= 0 && entity_id < MAX_ENTITIES);
    components->keyboard_input_components[entity_id].movement_direction_keys.up = movement_direction_keys.up;
    components->keyboard_input_components[entity_id].movement_direction_keys.down = movement_direction_keys.down;
    components->keyboard_input_components[entity_id].movement_direction_keys.left = movement_direction_keys.left;
    components->keyboard_input_components[entity_id].movement_direction_keys.right = movement_direction_keys.right;

    add_component_signature_to_entity(entity_id, Keyboard_Input_Component_Signature);
}
void add_velocity_component_to_entity(int entity_id, float speed)
{
    assert(entity_id >= 0 && entity_id < MAX_ENTITIES);
    components->velocity_components[entity_id].x = 0;
    components->velocity_components[entity_id].y = 0;
    components->velocity_components[entity_id].speed = speed;
    add_component_signature_to_entity(entity_id, Velocity_Component_Signature);
}

void add_health_component_to_entity(int entity_id, int max_health)
{
    components->health_components[entity_id].max_health = max_health;
    components->health_components[entity_id].health = components->health_components[entity_id].max_health;
    add_component_signature_to_entity(entity_id, Health_Component_Signature);
}
void add_collision_component_to_entity(int entity_id, float offset_x, float offset_y, float width, float height, bool is_static, uint32_t layer, uint32_t mask)
{
    components->collision_components[entity_id].height = height;
    components->collision_components[entity_id].width = width;
    components->collision_components[entity_id].offset_x = offset_x;
    components->collision_components[entity_id].offset_y = offset_y;

    components->collision_components[entity_id].is_static = is_static;

    components->collision_components[entity_id].layer = layer;
    components->collision_components[entity_id].mask = mask;

    add_component_signature_to_entity(entity_id, Collision_Component_Signature);
}