#include <stdbool.h>

#define TRUE 1
#define FALSE 0
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define MOVEMENT_SPEED_IN_PIXELS 50

// TODO Refactor this to be a component
typedef struct KeyboardInputComponent
{
    int entity_id;
    bool up;
    bool down;
    bool right;
    bool left;
} KeyboardInput;

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
    KeyboardInput keyboard_input_components[100];
    int total_position_components;
    int total_sprite_components;
    int total_keyboard_input_components;
} ComponentLists;