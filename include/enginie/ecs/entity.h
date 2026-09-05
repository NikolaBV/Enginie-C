#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "enginie/core/config.h"
#include "enginie/ecs/components.h"

extern int number_of_entities;
extern int signatures[MAX_ENTITIES];

extern ComponentLists *components;

int create_entity(void);

void add_component_signature_to_entity(int entity_id, ComponentSignatures component_signature);
bool does_entity_have_component(int entity_id, ComponentSignatures component_signature);

void add_position_component_to_entity(int entity_id, float x, float y);
void add_sprite_component_to_entity(int entity_id, uint32_t texture_id, uint32_t sprite_width, uint32_t sprite_height, int scale);
void add_keyboard_input_component_to_entity(int entity_id, Movement_Direction_Keys movement_direction_keys);
void add_animation_component_to_entity(int entity_id);
void add_velocity_component_to_entity(int entity_id, float speed);
void add_facing_component_to_entity(int entity_id);
void add_health_component_to_entity(int entity_id, int max_health);
void add_collision_component_to_entity(int entity_id, float offset_x, float offset_y, float width, float height);
