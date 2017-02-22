#include "atlas.h"
#include "error_logs.h"

TextureAtlas * init_texture_atlas(SDL_Renderer * rend, SDL_Surface * surf,
    uint32_t tile_w, uint32_t tile_h, uint32_t n_tiles_x, uint32_t n_tiles_y)
{
    TextureAtlas * ta = (TextureAtlas *) malloc(sizeof(TextureAtlas));
    if (ta == NULL) {
        return NULL;
    }

    ta->tex = SDL_CreateTextureFromSurface(rend, surf);
    if (ta->tex == NULL) {
        log_sdl_error("SDL_CreateTextureFromSurface");
        free(ta);
        return NULL;
    }

    ta->tile_w = tile_w;
    ta->tile_h = tile_h;
    ta->n_tiles_x = n_tiles_x;
    ta->n_tiles_y = n_tiles_y;

    return ta;
}


SDL_Rect * get_tile_rect(TextureAtlas * ta, int tile_number)
{
    SDL_Rect * rect = (SDL_Rect *) malloc(sizeof(SDL_Rect));
    if (rect == NULL) {
        return NULL;
    }

    // Todo
}
