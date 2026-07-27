
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <SDL3/SDL_render.h>
#include <SDL3_image/SDL_image.h>
#include "../src/constants.h"

#define POSITION_COMPONENT_SIGNATURE (1 << 0)
#define SPRITE_COMPONENT_SIGNATURE (1 << 1)
#define KEYBOARD_INPUT_COMPONENT_SIGNATURE (1 << 2)
#define ANIMATION_COMPONENT_SIGNATURE (1 << 3)

typedef enum
{
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    IDLE,
    NONE
} AnimationName;

extern int player_entity_id;
extern int ENTITIES;
extern int signatures[100];
extern SDL_Texture *textures[100];
extern uint32_t textures_count;

typedef enum
{
    PositionComponentSignature = POSITION_COMPONENT_SIGNATURE,
    SpriteComponentSignature = SPRITE_COMPONENT_SIGNATURE,
    KeyboardInputComponentSignature = KEYBOARD_INPUT_COMPONENT_SIGNATURE,
    AnimationComponentSignature = ANIMATION_COMPONENT_SIGNATURE
} ComponentSignatures;

typedef struct AnimationComponent
{
    int entity_id;
    int frame_index;
    float frame_width;
    float animation_period;
    float animation_time;
    AnimationName active_animation;
} AnimationComponent;

typedef struct
{
    bool up;
    bool down;
    bool right;
    bool left;
} Movement_Direction;

typedef struct
{
    Movement_Direction movement_direction;
    int entity_id;

} KeyboardInputComponent;

typedef struct
{
    int entity_id;
    float x;
    float y;
} PositionComponent;

typedef struct
{
    int entity_id;

    uint32_t texture_id;
    uint32_t texture_width;
    uint32_t texture_height;

    uint32_t sprite_width;
    uint32_t sprite_height;
} SpriteComponent;

typedef struct
{
    PositionComponent position_components[100];
    SpriteComponent sprite_components[100];
    KeyboardInputComponent keyboard_input_components[100];
    AnimationComponent animation_components[100];

    int total_position_components;
    int total_sprite_components;
    int total_keyboard_input_components;
    int total_animation_components;
} ComponentLists;

extern ComponentLists components;

void create_entity(float x, float y, uint32_t texture_id, uint32_t texture_width, uint32_t texture_height, uint32_t sprite_width, uint32_t sprite_height, ComponentLists *components);
void update_position_system(PositionComponent *position, KeyboardInputComponent *keyboardInput, ComponentLists *components, float deltaTime);
void update_render_system(SpriteComponent *sprite, PositionComponent *position, ComponentLists *components, SDL_Renderer *renderer);
void add_component_signature_to_entity(int entity_id, ComponentSignatures componentSignature);

void add_position_component_to_components(int entity_id, float x, float y, ComponentLists *components);
void add_sprite_component_to_components(int entity_id, uint32_t texture_id, uint32_t texture_width, uint32_t texture_height, uint32_t sprite_width, uint32_t sprite_height, ComponentLists *components);
void add_keyboard_input_component_to_components(int entity_id, ComponentLists *components);
void add_animation_component_to_entity(int entity_id, float frame_width);
void update_animation_system(AnimationComponent *animation, SpriteComponent *sprite, SDL_Renderer *renderer, float deltaTime);

uint32_t load_image_as_texture(char *path_to_image, SDL_Renderer *renderer);
SDL_Texture *get_texture_by_texture_id(uint32_t texture_id);
bool does_entity_have_component(int entity_id, ComponentSignatures componentSignature);