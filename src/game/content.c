#include <stdio.h>

#include "enginie/platform/texture_store.h"
#include "game/content.h"

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
