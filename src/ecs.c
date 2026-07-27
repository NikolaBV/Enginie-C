#include "../lib/ecs.h"

int player_entity_id = 0;
int ENTITIES = 0;
int signatures[100];
SDL_Texture *textures[100];
uint32_t textures_count = 0;
ComponentLists components;

void create_entity(float x, float y, uint32_t texture_id, uint32_t texture_width, uint32_t texture_height, uint32_t sprite_width, uint32_t sprite_height, ComponentLists *components)
{
    int id = ENTITIES++;

    add_position_component_to_components(id, x, y, components);
    add_sprite_component_to_components(id, texture_id, texture_width, texture_height, sprite_width, sprite_height, components);
}

void add_position_component_to_components(int entity_id, float x, float y, ComponentLists *components)
{
    components->position_components[entity_id].entity_id = entity_id;
    components->position_components[entity_id].x = x;
    components->position_components[entity_id].y = y;
    add_component_signature_to_entity(entity_id, POSITION_COMPONENT_SIGNATURE);
    components->total_position_components++;
}
void add_sprite_component_to_components(int entity_id, uint32_t texture_id, uint32_t texture_width, uint32_t texture_height, uint32_t sprite_width, uint32_t sprite_height, ComponentLists *components)
{
    components->sprite_components[entity_id].entity_id = entity_id;
    components->sprite_components[entity_id].sprite_height = sprite_height;
    components->sprite_components[entity_id].sprite_width = sprite_width;

    components->sprite_components[entity_id].texture_id = texture_id;
    components->sprite_components[entity_id].texture_width = texture_width;
    components->sprite_components[entity_id].texture_height = texture_height;

    add_component_signature_to_entity(entity_id, SPRITE_COMPONENT_SIGNATURE);
    components->total_sprite_components++;
}

void add_animation_component_to_entity(int entity_id, float frame_width)
{
    components.animation_components[entity_id].entity_id = entity_id;
    components.animation_components[entity_id].frame_width = frame_width;
    components.animation_components[entity_id].active_animation = NONE;
    components.animation_components[entity_id].frame_index = 0;
    components.animation_components[entity_id].animation_time = 0;
    components.animation_components[entity_id].animation_period = 0.1f;
    add_component_signature_to_entity(entity_id, AnimationComponentSignature);
}

void add_keyboard_input_component_to_components(int entity_id, ComponentLists *components)
{
    components->keyboard_input_components[entity_id].entity_id = entity_id;
    add_component_signature_to_entity(entity_id, KEYBOARD_INPUT_COMPONENT_SIGNATURE);
}

void update_position_system(PositionComponent *position, KeyboardInputComponent *keyboardInput, ComponentLists *components, float deltaTime)
{
    if (keyboardInput->movement_direction.up)
    {
        position->y -= MOVEMENT_SPEED_IN_PIXELS * deltaTime;
    }
    if (keyboardInput->movement_direction.down)
    {
        position->y += MOVEMENT_SPEED_IN_PIXELS * deltaTime;
    }
    if (keyboardInput->movement_direction.right)
    {
        position->x += MOVEMENT_SPEED_IN_PIXELS * deltaTime;
    }
    if (keyboardInput->movement_direction.left)
    {
        position->x -= MOVEMENT_SPEED_IN_PIXELS * deltaTime;
    }
}
void update_render_system(SpriteComponent *sprite, PositionComponent *position, ComponentLists *components, SDL_Renderer *renderer)
{
    SDL_Texture *entity_texture = get_texture_by_texture_id(sprite->texture_id);
    float srcRect_x_position = 0;

    if (does_entity_have_component(sprite->entity_id, AnimationComponentSignature))
    {
        srcRect_x_position = components->animation_components[sprite->entity_id].frame_index * sprite->sprite_width;
    }

    SDL_FRect srcRect = {srcRect_x_position, 0, sprite->sprite_width, sprite->sprite_height};
    SDL_FRect destRect = {components->position_components[sprite->entity_id].x, components->position_components[sprite->entity_id].y, sprite->sprite_width, sprite->sprite_height};

    SDL_RenderTexture(renderer, entity_texture, &srcRect, &destRect);
}

SDL_Texture *get_texture_by_texture_id(uint32_t texture_id)
{
    return textures[texture_id];
}

void update_animation_system(AnimationComponent *animation, SpriteComponent *sprite, SDL_Renderer *renderer, float deltaTime)
{
    if (animation->active_animation != NONE)
    {
        animation->animation_time += deltaTime;

        if (animation->animation_time > animation->animation_period)
        {
            animation->frame_index = (animation->frame_index + 1) % 4;
            animation->animation_time = 0.f;
        }
    }
}

void add_component_signature_to_entity(int entity_id, ComponentSignatures componentSignature)
{
    signatures[entity_id] |= componentSignature;
}
bool does_entity_have_component(int entity_id, ComponentSignatures componentSignature)
{
    return signatures[entity_id] & componentSignature;
}
uint32_t load_image_as_texture(char *path_to_image, SDL_Renderer *renderer)
{

    SDL_Surface *image = IMG_Load(path_to_image);

    if (image == NULL)
    {
        fprintf(stderr, "Couldn't load image: %s\n", SDL_GetError());
    }

    SDL_Texture *newTexture = SDL_CreateTextureFromSurface(renderer, image);
    uint32_t id = textures_count++;
    textures[id] = newTexture;
    return id;
}