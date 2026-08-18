
#pragma once
#include "constants.h"
#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>
#include <math.h>

extern int number_of_entities;
extern int signatures[MAX_COMPONENTS_SIGNATURES];
extern SDL_Texture *textures[MAX_TEXTURES_COUNT];
extern uint32_t textures_count;

typedef enum
{
    Position_Component_Signature = (1 << 0),
    Sprite_Component_Signature = (1 << 1),
    Keyboard_Input_Component_Signature = (1 << 2),
    Animation_Component_Signature = (1 << 3),
    Velocity_Component_Signature = (1 << 4),
    Facing_Component_Signature = (1 << 5)

} ComponentSignatures;

typedef struct
{
    int row;
    int frame_count;
    float frame_duration;
    bool looping;
} AnimationClip;

static const AnimationClip player_clips[4];

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
    SDL_Scancode Up;
    SDL_Scancode Down;
    SDL_Scancode Left;
    SDL_Scancode Right;
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
    PositionComponent position_components[MAX_ENTITIES];
    SpriteComponent sprite_components[MAX_ENTITIES];
    KeyboardInputComponent keyboard_input_components[MAX_ENTITIES];
    AnimationComponent animation_components[MAX_ENTITIES];
    VelocityComponent velocity_components[MAX_ENTITIES];
    FacingComponent facing_components[MAX_ENTITIES];
} ComponentLists;

extern ComponentLists *components;

extern Movement_Direction_Keys wasd_layout;
extern Movement_Direction_Keys arrows_layout;

int create_entity(void);

void update_position_system(int entity_id, float delta_time);
void update_render_system(int entity_id, SDL_Renderer *renderer);
void update_animation_system(int entity_id, float delta_time);
void update_input_system(int entity_id);
void update_facing_system(int entity_id);

void add_component_signature_to_entity(int entity_id, ComponentSignatures componentSignature);

void add_position_component_to_entity(int entity_id, float x, float y);
void add_sprite_component_to_entity(int entity_id, uint32_t texture_id, uint32_t sprite_width, uint32_t sprite_height, int scale);
void add_keyboard_input_component_to_entity(int entity_id, Movement_Direction_Keys movement_direction_keys);
void add_animation_component_to_entity(int entity_id);
void add_velocity_component_to_entity(int entity_id, float speed);
void add_facing_component_entity(int entity_id);

uint32_t load_image_as_texture(char *path_to_image, SDL_Renderer *renderer);
SDL_Texture *get_texture_by_texture_id(uint32_t texture_id);
bool does_entity_have_component(int entity_id, ComponentSignatures componentSignature);