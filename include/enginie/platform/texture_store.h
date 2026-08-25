#pragma once

#include <SDL3/SDL_render.h>
#include <stdint.h>

#include "enginie/core/config.h"

extern SDL_Texture *textures[MAX_TEXTURES_COUNT];
extern uint32_t textures_count;

uint32_t load_image_as_texture(const char *path_to_image, SDL_Renderer *renderer);
uint32_t load_texture_or_fail(const char *path, SDL_Renderer *renderer);
SDL_Texture *get_texture_by_texture_id(uint32_t texture_id);
