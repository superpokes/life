#ifndef ATLAS_H
#define ATLAS_H

#include "SDL.h"
#include "stdint.h"

/*
 * Abstracts the necessary operations to use a single texture for many tiles.
 * Fields:
 *     tex (SDL_Texture *): texture used
 *     tile_w, tile_h (uint32_t): width and height of a tile
 *     n_tiles_x, n_tiles_y (uint32_t): number of tiles in a row in
 *         the x or y axis.
 */
typedef struct {
    SDL_Texture * tex;
    uint32_t tile_w;
    uint32_t tile_h;
    uint32_t n_tiles_x;
    uint32_t n_tiles_y;
} TextureAtlas;

/*
 *
 */
TextureAtlas * init_texture_atlas(SDL_Surface * surf, uint32_t tile_w,
    uint32_t tile_h, uint32_t n_tiles_w, uint32_t n_tiles_h);


/*
 *
 */
SDL_Rect * get_tile_rect(TextureAtlas * ta, int tile_number);


/*
 *
 */
void free_texture_atlas(TextureAtlas * texture_atlas);

#endif /* end of include guard: ATLAS_H */
