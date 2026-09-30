#pragma once

#include <SDL3/SDL_render.h>
#include <stdint.h>

#include "enginie/core/config.h"

extern SDL_Texture *textures[MAX_TEXTURES_COUNT];
extern uint32_t textures_count;

// TODO think about creating some sort of logic to programatically
// be able to add custom colors so these hardcoded enums are not needed
typedef struct
{
    Uint8 red;
    Uint8 green;
    Uint8 blue;
    Uint8 alpha;
} Color;

extern Color red;
extern Color green;
extern Color blue;
extern Color magenta;

uint32_t load_image_as_texture(const char *path_to_image, SDL_Renderer *renderer);
uint32_t load_texture_or_fail(const char *path, SDL_Renderer *renderer);
SDL_Texture *get_texture_by_texture_id(uint32_t texture_id);

void set_render_draw_color(Color color, SDL_Renderer *renderer);