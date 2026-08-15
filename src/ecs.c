#include "../lib/ecs.h"

int ENTITIES = 0;
int signatures[100];

static ComponentLists world_storage;
ComponentLists *components = &world_storage;

SDL_Texture *textures[100];
uint32_t textures_count = 0;

int create_entity(float x, float y, uint32_t texture_id, uint32_t texture_width, uint32_t texture_height, uint32_t sprite_width, uint32_t sprite_height, int scale)
{
    int id = ENTITIES++;

    add_position_component_to_components(id, x, y);
    add_sprite_component_to_components(id, texture_id, texture_width, texture_height, sprite_width, sprite_height, scale);

    return id;
}

void add_position_component_to_components(int entity_id, float x, float y)
{
    components->position_components[entity_id].x = x;
    components->position_components[entity_id].y = y;

    add_component_signature_to_entity(entity_id, POSITION_COMPONENT_SIGNATURE);
    components->total_position_components++;
}
void add_sprite_component_to_components(int entity_id, uint32_t texture_id, uint32_t texture_width, uint32_t texture_height, uint32_t sprite_width, uint32_t sprite_height, int scale)
{
    components->sprite_components[entity_id].sprite_height = sprite_height;
    components->sprite_components[entity_id].sprite_width = sprite_width;

    components->sprite_components[entity_id].texture_id = texture_id;
    components->sprite_components[entity_id].texture_width = texture_width;
    components->sprite_components[entity_id].texture_height = texture_height;

    components->sprite_components[entity_id].scale = scale;

    add_component_signature_to_entity(entity_id, SPRITE_COMPONENT_SIGNATURE);
    components->total_sprite_components++;
}

void add_animation_component_to_entity(int entity_id)
{
    components->animation_components[entity_id].frame_index = 0;
    components->animation_components[entity_id].animation_time = 0;
    components->animation_components[entity_id].animation_period = 0.1f;
    add_component_signature_to_entity(entity_id, AnimationComponentSignature);
}

void add_keyboard_input_component_to_components(int entity_id)
{
    add_component_signature_to_entity(entity_id, KeyboardInputComponentSignature);
}
void add_velocity_component_to_entity(int entity_id, float speed)
{
    components->velocity_components[entity_id].x = 1;
    components->velocity_components[entity_id].y = 1;
    components->velocity_components[entity_id].speed = speed;
    add_component_signature_to_entity(entity_id, VelocityComponentSignature);
}

void update_position_system(int entity_id, float deltaTime)
{
    PositionComponent *position = &components->position_components[entity_id];
    VelocityComponent *velocity = &components->velocity_components[entity_id];

    position->x += velocity->x * deltaTime;
    position->y += velocity->y * deltaTime;
}
void update_render_system(int entity_id, SDL_Renderer *renderer)
{
    // TODO Fix function to not require entity_id stored in the component
    SpriteComponent *sprite = &components->sprite_components[entity_id];
    PositionComponent *position = &components->position_components[entity_id];
    SDL_Texture *entity_texture = get_texture_by_texture_id(sprite->texture_id);
    float srcRect_x_position = 0;

    if (does_entity_have_component(entity_id, AnimationComponentSignature))
    {
        srcRect_x_position = components->animation_components[entity_id].frame_index * sprite->sprite_width;
    }

    SDL_FRect srcRect = {srcRect_x_position, 0, sprite->sprite_width, sprite->sprite_height};
    SDL_FRect destRect = {position->x, position->y, sprite->sprite_width * sprite->scale, sprite->sprite_height * sprite->scale};

    SDL_RenderTexture(renderer, entity_texture, &srcRect, &destRect);
}

void update_input_system(int entity_id)
{
    VelocityComponent *velocity = &components->velocity_components[entity_id];

    const bool *keys = SDL_GetKeyboardState(NULL);
    velocity->x = (keys[SDL_SCANCODE_D] - keys[SDL_SCANCODE_A]) * velocity->speed;
    velocity->y = (keys[SDL_SCANCODE_S] - keys[SDL_SCANCODE_W]) * velocity->speed;
}

void update_animation_system(int entity_id, float deltaTime)
{
    AnimationComponent *animation = &components->animation_components[entity_id];
    SpriteComponent *sprite = &components->sprite_components[entity_id];

    animation->animation_time += deltaTime;
    int frame_size = (sprite->texture_width / sprite->sprite_width);

    if (animation->animation_time > animation->animation_period)
    {
        animation->frame_index = (animation->frame_index + 1) % frame_size;
        animation->animation_time -= animation->animation_period;
    }
}

SDL_Texture *get_texture_by_texture_id(uint32_t texture_id)
{
    if (texture_id >= textures_count)
    {
        return NULL;
    }
    return textures[texture_id];
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
        return UINT32_MAX;
    }

    if (image == NULL)
    {
        fprintf(stderr, "Couldn't load image: %s\n", SDL_GetError());
        return UINT32_MAX;
    }

    if (textures_count >= 100)
    {
        fprintf(stderr, "Texture array is maxxed out\n");
        return UINT32_MAX;
    }
    SDL_Texture *newTexture = SDL_CreateTextureFromSurface(renderer, image);
    uint32_t id = textures_count++;
    textures[id] = newTexture;
    SDL_DestroySurface(image);
    return id;
}
