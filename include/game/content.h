#pragma once

#include <SDL3/SDL_render.h>
#include <stdbool.h>

#include "enginie/ecs/components.h"

typedef enum
{
    CLIP_IDLE,
    CLIP_WALK
} Clip_Type;

typedef enum
{
    PLANTS
} Assets_Paths;

extern const char *assets_paths[];

extern const int player_clip_count;
extern AnimationClip player_clips[];

extern Movement_Direction_Keys wasd_layout;
extern Movement_Direction_Keys arrows_layout;

bool load_player_clips(SDL_Renderer *renderer);
