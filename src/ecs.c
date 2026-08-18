#include "../lib/ecs.h"

int number_of_entities = 0;
int signatures[100];

static ComponentLists world_storage;
ComponentLists *components = &world_storage;

SDL_Texture *textures[MAX_TEXTURES_COUNT];
uint32_t textures_count = 0;

static const AnimationClip player_clips[] = {
    {0, 4, 0.15f, true}, // idle
    {1, 6, 0.08f, true}, // walk
};

int create_entity()
{
    int id = number_of_entities++;
    return id;
}

void add_position_component_to_entity(int entity_id, float x, float y)
{
    components->position_components[entity_id].x = x;
    components->position_components[entity_id].y = y;

    add_component_signature_to_entity(entity_id, Position_Component_Signature);
}
void add_sprite_component_to_entity(int entity_id, uint32_t texture_id, uint32_t sprite_width, uint32_t sprite_height, int scale)
{
    components->sprite_components[entity_id].sprite_height = sprite_height;
    components->sprite_components[entity_id].sprite_width = sprite_width;
    components->sprite_components[entity_id].texture_id = texture_id;
    components->sprite_components[entity_id].scale = scale;

    add_component_signature_to_entity(entity_id, Sprite_Component_Signature);
}

void add_animation_component_to_entity(int entity_id)
{
    components->animation_components[entity_id].clips = player_clips;
    components->animation_components[entity_id].active_clip = 0;
    components->animation_components[entity_id].frame_index = 0;
    components->animation_components[entity_id].time_in_frame = 0;
    components->animation_components[entity_id].finished = false;

    add_component_signature_to_entity(entity_id, Animation_Component_Signature);
}
void add_facing_component_entity(int entity_id)
{
    components->facing_components[entity_id].facing = FACE_SIDE;
    components->facing_components[entity_id].flip = false;
    add_component_signature_to_entity(entity_id, Facing_Component_Signature);
}

void add_keyboard_input_component_to_entity(int entity_id)
{
    add_component_signature_to_entity(entity_id, Keyboard_Input_Component_Signature);
}
void add_velocity_component_to_entity(int entity_id, float speed)
{
    components->velocity_components[entity_id].x = 1;
    components->velocity_components[entity_id].y = 1;
    components->velocity_components[entity_id].speed = speed;
    add_component_signature_to_entity(entity_id, Velocity_Component_Signature);
}

void update_position_system(int entity_id, float delta_time)
{
    PositionComponent *position = &components->position_components[entity_id];
    VelocityComponent *velocity = &components->velocity_components[entity_id];

    position->x += velocity->x * delta_time;
    position->y += velocity->y * delta_time;
}
void update_render_system(int entity_id, SDL_Renderer *renderer)
{
    SpriteComponent *sprite = &components->sprite_components[entity_id];
    PositionComponent *position = &components->position_components[entity_id];
    FacingComponent *facing = &components->facing_components[entity_id];

    SDL_Texture *entity_texture = get_texture_by_texture_id(sprite->texture_id);
    float srcRect_x_position = 0;

    if (does_entity_have_component(entity_id, Animation_Component_Signature))
    {
        srcRect_x_position = components->animation_components[entity_id].frame_index * sprite->sprite_width;
    }

    SDL_FRect srcRect = {srcRect_x_position, facing->facing * sprite->sprite_height, sprite->sprite_width, sprite->sprite_height};
    SDL_FRect destRect = {position->x, position->y, sprite->sprite_width * sprite->scale, sprite->sprite_height * sprite->scale};

    SDL_RenderTextureRotated(renderer, entity_texture, &srcRect, &destRect, 0.0, NULL, facing->flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}

void update_input_system(int entity_id)
{
    VelocityComponent *velocity = &components->velocity_components[entity_id];

    const bool *keys = SDL_GetKeyboardState(NULL);

    float direction_x = (keys[SDL_SCANCODE_D] - keys[SDL_SCANCODE_A]);
    float direction_y = (keys[SDL_SCANCODE_S] - keys[SDL_SCANCODE_W]);

    float length_of_movement_vector = sqrtf(direction_x * direction_x + direction_y * direction_y);
    if (length_of_movement_vector > 0.0001f)
    {
        direction_x /= length_of_movement_vector;
        direction_y /= length_of_movement_vector;
    }

    velocity->x = direction_x * velocity->speed;
    velocity->y = direction_y * velocity->speed;
}

void update_animation_system(int entity_id, float delta_time)
{
    AnimationComponent *animation = &components->animation_components[entity_id];
    const AnimationClip *animation_clip = &animation->clips[animation->active_clip];

    if (animation->finished)
        return;

    animation->time_in_frame += delta_time;
    while (animation->time_in_frame >= animation_clip->frame_duration)
    {
        animation->time_in_frame -= animation_clip->frame_duration;
        animation->frame_index++;

        if (animation->frame_index >= animation_clip->frame_count)
        {
            if (animation_clip->looping)
            {
                animation->frame_index = 0;
            }
            else
            {
                animation->frame_index = animation_clip->frame_count - 1;
                animation->finished = true;
            }
        }
    }
}

void update_facing_system(int entity_id)
{
    VelocityComponent *velocity = &components->velocity_components[entity_id];
    FacingComponent *facing = &components->facing_components[entity_id];

    if (fabsf(velocity->x) < 0.001f && fabsf(velocity->y) < 0.001f)
        return;

    if (fabsf(velocity->x) > fabsf(velocity->y))
    {
        facing->facing = FACE_SIDE;
        facing->flip = (velocity->x < 0);
    }
    else
        facing->facing = (velocity->y > 0) ? FACE_DOWN : FACE_UP;
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

    if (textures_count >= MAX_TEXTURES_COUNT)
    {
        fprintf(stderr, "Texture array is maxxed out\n");
        return UINT32_MAX;
    }
    SDL_Texture *newTexture = SDL_CreateTextureFromSurface(renderer, image);

    if (newTexture == NULL)
    {
        fprintf(stderr, "Couldn't create texture from surface: %s\n", SDL_GetError());
        return UINT32_MAX;
    }

    uint32_t id = textures_count++;
    textures[id] = newTexture;
    SDL_DestroySurface(image);
    return id;
}
