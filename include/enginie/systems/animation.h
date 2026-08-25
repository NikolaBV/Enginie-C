#pragma once

#include "enginie/ecs/components.h"

void animation_play(AnimationComponent *animation, int clip);
void update_animation_system(int entity_id, float delta_time);
void update_animation_selection_system(int entity_id);
