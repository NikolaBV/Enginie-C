#pragma once

#include <SDL3/SDL_render.h>

void update_render_system(int entity_id, SDL_Renderer *renderer);
void render_collider_debug(int entity_id, SDL_Renderer *renderer);
