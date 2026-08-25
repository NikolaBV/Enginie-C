#include <SDL3_image/SDL_image.h>
#include <stdio.h>

#include "enginie/platform/texture_store.h"

SDL_Texture *textures[MAX_TEXTURES_COUNT];
uint32_t textures_count = 0;

SDL_Texture *get_texture_by_texture_id(uint32_t texture_id)
{
    if (texture_id >= textures_count)
    {
        return NULL;
    }
    return textures[texture_id];
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
