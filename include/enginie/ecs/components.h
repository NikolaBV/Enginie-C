#pragma once

#include <SDL3/SDL_scancode.h>
#include <stdbool.h>
#include <stdint.h>

#include "enginie/core/config.h"

typedef enum
{
    Position_Component_Signature = (1 << 0),
    Sprite_Component_Signature = (1 << 1),
    Keyboard_Input_Component_Signature = (1 << 2),
    Animation_Component_Signature = (1 << 3),
    Velocity_Component_Signature = (1 << 4),
    Facing_Component_Signature = (1 << 5),
    Health_Component_Signature = (1 << 6),
    Collision_Component_Signature = (1 << 7)

} ComponentSignatures;

typedef struct
{
    uint32_t texture_id;
    int frame_count;
    float frame_duration;
    bool looping;
} AnimationClip;

typedef struct
{
    const AnimationClip *clips;
    int clip_count;
    int active_clip;
    int frame_index;
    float time_in_frame;
    bool finished;
} AnimationComponent;

typedef struct
{
    SDL_Scancode up;
    SDL_Scancode down;
    SDL_Scancode left;
    SDL_Scancode right;
} Movement_Direction_Keys;

typedef struct
{
    Movement_Direction_Keys movement_direction_keys;
} KeyboardInputComponent;

typedef struct
{
    float x;
    float y;
} PositionComponent;

typedef struct
{
    int scale;

    uint32_t texture_id;
    uint32_t sprite_width;
    uint32_t sprite_height;

} SpriteComponent;

typedef struct
{
    float x;
    float y;
    float speed;
} VelocityComponent;

typedef enum
{
    FACE_DOWN,
    FACE_UP,
    FACE_SIDE
} Facing;

typedef struct
{
    Facing facing;
    bool flip;
} FacingComponent;

typedef struct
{
    int health;
    int max_health;
} HealthComponent;

typedef struct
{
    float offset_x, offset_y;
    float width, height;
} CollisionComponent;

typedef struct
{
    PositionComponent position_components[MAX_ENTITIES];
    SpriteComponent sprite_components[MAX_ENTITIES];
    KeyboardInputComponent keyboard_input_components[MAX_ENTITIES];
    AnimationComponent animation_components[MAX_ENTITIES];
    VelocityComponent velocity_components[MAX_ENTITIES];
    FacingComponent facing_components[MAX_ENTITIES];
    HealthComponent health_components[MAX_ENTITIES];
    CollisionComponent collision_components[MAX_ENTITIES];

} ComponentLists;
