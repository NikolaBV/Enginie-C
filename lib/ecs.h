
#include <stdbool.h>
#include <stdint.h>
#include <SDL3/SDL_render.h>
#include "../src/constants.h"

#define POSITION_COMPONENT_SIGNATURE (1 << 0)
#define SPRITE_COMPONENT_SIGNATURE (1 << 1)
#define KEYBOARD_INPUT_COMPONENT_SIGNATURE (1 << 2)

extern int player_entity_id;
extern int ENTITIES;
extern int signatures[100];

typedef enum
{
    PositionComponentSignature = POSITION_COMPONENT_SIGNATURE,
    SpriteComponentSignature = SPRITE_COMPONENT_SIGNATURE,
    KeyboardInputComponentSignature = KEYBOARD_INPUT_COMPONENT_SIGNATURE
} ComponentSignatures;

typedef struct
{
    int entity_id;
    bool up;
    bool down;
    bool right;
    bool left;
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
    int total_position_components;
    int total_sprite_components;
    int total_keyboard_input_components;
} ComponentLists;

extern ComponentLists components;

void create_entity(float x, float y, uint32_t texture_width, uint32_t texture_height, uint32_t sprite_width, uint32_t sprite_height, ComponentLists *components);
void update_position_system(PositionComponent *position, KeyboardInputComponent *keyboardInput, ComponentLists *components, float deltaTime);
void update_render_system(SpriteComponent *sprite, PositionComponent *position, ComponentLists *components, SDL_Renderer *renderer);
void add_component_signature_to_entity(int entity_id, ComponentSignatures componentSignature);

void add_position_component_to_components(int entity_id, float x, float y, ComponentLists *components);
void add_sprite_component_to_components(int entity_id, uint32_t texture_width, uint32_t texture_height, uint32_t sprite_width, uint32_t sprite_height, ComponentLists *components);
bool does_entity_have_component(int entity_id, ComponentSignatures componentSignature);