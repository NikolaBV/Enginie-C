#include "ecs.h"
#include <SDL3_image/SDL_image.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>

int number_of_entities = 0;
int signatures[MAX_ENTITIES];

static ComponentLists world_storage;
ComponentLists *components = &world_storage;

SDL_Texture *textures[MAX_TEXTURES_COUNT];
uint32_t textures_count = 0;

Movement_Direction_Keys wasd_layout = {
    SDL_SCANCODE_W,
    SDL_SCANCODE_S,
    SDL_SCANCODE_A,
    SDL_SCANCODE_D,

};
Movement_Direction_Keys arrows_layout = {
    SDL_SCANCODE_UP,
    SDL_SCANCODE_DOWN,
    SDL_SCANCODE_LEFT,
    SDL_SCANCODE_RIGHT,

};

static const char *player_clip_paths[] = {
    [CLIP_IDLE] = "assets/idle.png",
    [CLIP_WALK] = "assets/walk.png",
};

const char *assets_paths[] = {
    [PLANTS] = "assets/plants.png",
};
AnimationClip player_clips[] = {
    [CLIP_IDLE] = {UINT32_MAX, 4, 0.15f, true},
    [CLIP_WALK] = {UINT32_MAX, 6, 0.08f, true},
};
const int player_clip_count = (int)(sizeof player_clips / sizeof player_clips[0]);

bool load_player_clips(SDL_Renderer *renderer)
{
    for (int i = 0; i < player_clip_count; i++)
    {
        uint32_t id = load_image_as_texture(player_clip_paths[i], renderer);
        if (id == UINT32_MAX)
        {
            fprintf(stderr, "Couldn't load clip texture %s\n", player_clip_paths[i]);
            return false;
        }
        player_clips[i].texture_id = id;
    }
    return true;
}

int create_entity(void)
{
    if (number_of_entities >= MAX_ENTITIES)
    {
        fprintf(stderr, "Entity limit reached (%d)\n", MAX_ENTITIES);
        return -1;
    }
    return number_of_entities++;
}

void add_position_component_to_entity(int entity_id, float x, float y)
{
    assert(entity_id >= 0 && entity_id < MAX_ENTITIES);
    components->position_components[entity_id].x = x;
    components->position_components[entity_id].y = y;

    add_component_signature_to_entity(entity_id, Position_Component_Signature);
}
void add_sprite_component_to_entity(int entity_id, uint32_t texture_id, uint32_t sprite_width, uint32_t sprite_height, int scale)
{
    assert(entity_id >= 0 && entity_id < MAX_ENTITIES);
    components->sprite_components[entity_id].sprite_height = sprite_height;
    components->sprite_components[entity_id].sprite_width = sprite_width;
    components->sprite_components[entity_id].texture_id = texture_id;

    // default the scaler to a value that keeps the sprite unchanged if a number 0 or under is passed
    if (scale <= 0)
    {
        scale = 1;
    }
    components->sprite_components[entity_id].scale = scale;

    add_component_signature_to_entity(entity_id, Sprite_Component_Signature);
}

void add_animation_component_to_entity(int entity_id)
{
    assert(entity_id >= 0 && entity_id < MAX_ENTITIES);
    components->animation_components[entity_id].clips = player_clips;
    components->animation_components[entity_id].active_clip = 0;
    components->animation_components[entity_id].frame_index = 0;
    components->animation_components[entity_id].time_in_frame = 0;
    components->animation_components[entity_id].finished = false;
    components->animation_components[entity_id].clip_count = player_clip_count;

    add_component_signature_to_entity(entity_id, Animation_Component_Signature);
}
void add_facing_component_to_entity(int entity_id)
{
    assert(entity_id >= 0 && entity_id < MAX_ENTITIES);
    components->facing_components[entity_id].facing = FACE_DOWN;
    components->facing_components[entity_id].flip = false;
    add_component_signature_to_entity(entity_id, Facing_Component_Signature);
}

void add_keyboard_input_component_to_entity(int entity_id, Movement_Direction_Keys movement_direction_keys)
{
    assert(entity_id >= 0 && entity_id < MAX_ENTITIES);
    components->keyboard_input_components[entity_id].movement_direction_keys.up = movement_direction_keys.up;
    components->keyboard_input_components[entity_id].movement_direction_keys.down = movement_direction_keys.down;
    components->keyboard_input_components[entity_id].movement_direction_keys.left = movement_direction_keys.left;
    components->keyboard_input_components[entity_id].movement_direction_keys.right = movement_direction_keys.right;

    add_component_signature_to_entity(entity_id, Keyboard_Input_Component_Signature);
}
void add_velocity_component_to_entity(int entity_id, float speed)
{
    assert(entity_id >= 0 && entity_id < MAX_ENTITIES);
    components->velocity_components[entity_id].x = 0;
    components->velocity_components[entity_id].y = 0;
    components->velocity_components[entity_id].speed = speed;
    add_component_signature_to_entity(entity_id, Velocity_Component_Signature);
}

void update_position_system(int entity_id, float delta_time)
{
    assert(entity_id >= 0 && entity_id < MAX_ENTITIES);

    if (does_entity_have_component(entity_id, Position_Component_Signature) && does_entity_have_component(entity_id, Velocity_Component_Signature))
    {
        PositionComponent *position = &components->position_components[entity_id];
        VelocityComponent *velocity = &components->velocity_components[entity_id];

        position->x += velocity->x * delta_time;
        position->y += velocity->y * delta_time;
    }
}
void update_render_system(int entity_id, SDL_Renderer *renderer)
{
    assert(entity_id >= 0 && entity_id < MAX_ENTITIES);

    if (does_entity_have_component(entity_id, Sprite_Component_Signature) && does_entity_have_component(entity_id, Position_Component_Signature))
    {
        SpriteComponent *sprite = &components->sprite_components[entity_id];
        PositionComponent *position = &components->position_components[entity_id];

        Facing facing_row = FACE_DOWN;
        bool flip = false;

        if (does_entity_have_component(entity_id, Facing_Component_Signature))
        {
            facing_row = components->facing_components[entity_id].facing;
            flip = components->facing_components[entity_id].flip;
        }

        SDL_Texture *entity_texture = NULL;
        float src_rect_x = 0;

        if (does_entity_have_component(entity_id, Animation_Component_Signature))
        {
            AnimationComponent *animation = &components->animation_components[entity_id];

            src_rect_x = animation->frame_index * sprite->sprite_width;
            entity_texture = get_texture_by_texture_id(animation->clips[animation->active_clip].texture_id);
        }
        else
        {
            entity_texture = get_texture_by_texture_id(sprite->texture_id);
        }

        SDL_FRect src_rect = {src_rect_x, facing_row * sprite->sprite_height, sprite->sprite_width, sprite->sprite_height};
        SDL_FRect dest_rect = {position->x, position->y, sprite->sprite_width * sprite->scale, sprite->sprite_height * sprite->scale};

        SDL_RenderTextureRotated(renderer, entity_texture, &src_rect, &dest_rect, 0.0, NULL, flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
    }
}

void update_input_system(int entity_id)
{
    assert(entity_id >= 0 && entity_id < MAX_ENTITIES);

    if (does_entity_have_component(entity_id, Keyboard_Input_Component_Signature) && does_entity_have_component(entity_id, Velocity_Component_Signature))
    {
        VelocityComponent *velocity = &components->velocity_components[entity_id];
        KeyboardInputComponent *keyboard_input = &components->keyboard_input_components[entity_id];

        const bool *keys = SDL_GetKeyboardState(NULL);

        float direction_x = (keys[keyboard_input->movement_direction_keys.right] - keys[keyboard_input->movement_direction_keys.left]);
        float direction_y = (keys[keyboard_input->movement_direction_keys.down] - keys[keyboard_input->movement_direction_keys.up]);

        float length_of_movement_vector = sqrtf(direction_x * direction_x + direction_y * direction_y);
        if (length_of_movement_vector > 0.0001f)
        {
            direction_x /= length_of_movement_vector;
            direction_y /= length_of_movement_vector;
        }

        velocity->x = direction_x * velocity->speed;
        velocity->y = direction_y * velocity->speed;
    }
}

void update_animation_system(int entity_id, float delta_time)
{
    assert(entity_id >= 0 && entity_id < MAX_ENTITIES);

    if (does_entity_have_component(entity_id, Animation_Component_Signature))
    {
        AnimationComponent *animation = &components->animation_components[entity_id];
        const AnimationClip *animation_clip = &animation->clips[animation->active_clip];

        if (animation->finished)
            return;

        animation->time_in_frame += delta_time;
        assert(animation_clip->frame_duration > 0.0f);
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
}

void update_facing_system(int entity_id)
{
    assert(entity_id >= 0 && entity_id < MAX_ENTITIES);
    if (does_entity_have_component(entity_id, Facing_Component_Signature) && does_entity_have_component(entity_id, Velocity_Component_Signature))
    {
        VelocityComponent *velocity = &components->velocity_components[entity_id];
        FacingComponent *facing = &components->facing_components[entity_id];

        if (fabsf(velocity->x) < 0.001f && fabsf(velocity->y) < 0.001f)
            return;

        if (fabsf(velocity->x) >= fabsf(velocity->y))
        {
            facing->facing = FACE_SIDE;
            facing->flip = (velocity->x < 0);
        }
        else
        {
            facing->facing = (velocity->y > 0) ? FACE_DOWN : FACE_UP;
        }
    }
}
void animation_play(AnimationComponent *animation, int clip)
{
    assert(clip >= 0 && clip < animation->clip_count);

    if (animation->active_clip == clip)
        return;

    animation->active_clip = clip;
    animation->frame_index = 0;
    animation->time_in_frame = 0.0f;
    animation->finished = false;
}
void update_animation_selection_system(int entity_id)
{
    assert(entity_id >= 0 && entity_id < MAX_ENTITIES);

    if (does_entity_have_component(entity_id, Velocity_Component_Signature) && does_entity_have_component(entity_id, Animation_Component_Signature))
    {
        VelocityComponent *velocity = &components->velocity_components[entity_id];
        AnimationComponent *animation = &components->animation_components[entity_id];

        bool is_moving = (fabsf(velocity->x) > 0.001f || fabsf(velocity->y) > 0.001f);
        animation_play(animation, is_moving ? CLIP_WALK : CLIP_IDLE);
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

void add_component_signature_to_entity(int entity_id, ComponentSignatures component_signature)
{
    assert(entity_id >= 0 && entity_id < MAX_ENTITIES);
    signatures[entity_id] |= component_signature;
}
bool does_entity_have_component(int entity_id, ComponentSignatures component_signature)
{
    assert(entity_id >= 0 && entity_id < MAX_ENTITIES);
    return signatures[entity_id] & component_signature;
}
uint32_t load_image_as_texture(const char *path_to_image, SDL_Renderer *renderer)
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
        SDL_DestroySurface(image);
        return UINT32_MAX;
    }
    SDL_Texture *new_texture = SDL_CreateTextureFromSurface(renderer, image);

    if (new_texture == NULL)
    {
        fprintf(stderr, "Couldn't create texture from surface: %s\n", SDL_GetError());
        SDL_DestroySurface(image);
        return UINT32_MAX;
    }
    SDL_SetTextureScaleMode(new_texture, SDL_SCALEMODE_NEAREST);

    uint32_t id = textures_count++;
    textures[id] = new_texture;
    SDL_DestroySurface(image);
    return id;
}
uint32_t load_texture_or_fail(const char *path, SDL_Renderer *renderer)
{
    uint32_t id = load_image_as_texture(path, renderer);
    if (id == UINT32_MAX)
    {
        fprintf(stderr, "Couldn't load %s\n", path);
        return UINT32_MAX;
    }

    SDL_Texture *texture = get_texture_by_texture_id(id);
    if (texture == NULL)
    {
        fprintf(stderr, "Couldn't look up texture for %s\n", path);
        return UINT32_MAX;
    }

    return id;
}