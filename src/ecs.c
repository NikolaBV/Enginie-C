#include "../lib/ecs.h"

int player_entity_id = 0;
int ENTITIES = 0;
int signatures[100];
ComponentLists components;

void create_entity(float x, float y, uint32_t texture_width, uint32_t texture_height, uint32_t sprite_width, uint32_t sprite_height, ComponentLists *components)
{
    int id = ENTITIES++;

    add_position_component_to_components(id, x, y, components);
    add_sprite_component_to_components(id, texture_width, texture_height, sprite_width, sprite_height, components);
}

void add_position_component_to_components(int entity_id, float x, float y, ComponentLists *components)
{
    components->position_components[entity_id].entity_id = entity_id;
    components->position_components[entity_id].x = x;
    components->position_components[entity_id].y = y;
    signatures[entity_id] |= POSITION_COMPONENT_SIGNATURE;
    components->total_position_components++;
}
void add_sprite_component_to_components(int entity_id, uint32_t texture_width, uint32_t texture_height, uint32_t sprite_width, uint32_t sprite_height, ComponentLists *components)
{
    components->sprite_components[entity_id].entity_id = entity_id;
    components->sprite_components[entity_id].sprite_height = sprite_height;
    components->sprite_components[entity_id].sprite_width = sprite_width;
    components->sprite_components[entity_id].texture_width = texture_width;
    components->sprite_components[entity_id].texture_height = texture_height;
    signatures[entity_id] |= SPRITE_COMPONENT_SIGNATURE;
    components->total_sprite_components++;
}

void update_position_system(PositionComponent *position, KeyboardInputComponent *keyboardInput, ComponentLists *components, float deltaTime)
{
    if (keyboardInput->up)
    {
        position->y -= MOVEMENT_SPEED_IN_PIXELS * deltaTime;
    }
    if (keyboardInput->down)
    {
        position->y += MOVEMENT_SPEED_IN_PIXELS * deltaTime;
    }
    if (keyboardInput->right)
    {
        position->x += MOVEMENT_SPEED_IN_PIXELS * deltaTime;
    }
    if (keyboardInput->left)
    {
        position->x -= MOVEMENT_SPEED_IN_PIXELS * deltaTime;
    }
}
void update_render_system(SpriteComponent *sprite, PositionComponent *position, ComponentLists *components, SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_FRect entity_rect = {position->x, position->y, sprite->sprite_width, sprite->sprite_height};
    SDL_RenderFillRect(renderer, &entity_rect);
}

void add_component_signature_to_entity(int entity_id, ComponentSignatures componentSignature)
{
    signatures[entity_id] |= componentSignature;
}
bool does_entity_have_component(int entity_id, ComponentSignatures componentSignature)
{
    return signatures[entity_id] & componentSignature;
}
