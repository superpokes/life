#include "life_gui.h"
#include "stdlib.h"
#include "error_logs.h"


life_gui_t * init_life_gui(int window_width, int window_height)
{
    life_gui_t * life_gui = (life_gui_t *) malloc(sizeof(life_gui_t));
    if (life_gui == NULL) {
        return NULL;
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        log_sdl_error("SDL_Init");
        return NULL;
    }

    life_gui->window = SDL_CreateWindow("Life ❤", 100, 100, window_width, window_height, SDL_WINDOW_SHOWN);
    if (life_gui->window == NULL) {
        log_sdl_error("SDL_CreateWindow");
        destroy_life_gui(life_gui);
        return NULL;
    }

    life_gui->renderer = SDL_CreateRenderer(life_gui->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (life_gui->renderer == NULL) {
        log_sdl_error("SDL_CreateRenderer");
        destroy_life_gui(life_gui);
        return NULL;
    }

    SDL_Surface * tileset_image = SDL_LoadBMP("../assets/Taffer.bmp");
    if (tileset_image == NULL) {
        log_sdl_error("SDL_LoadBMP");
        destroy_life_gui(life_gui);
        return NULL;
    }

    life_gui->tileset = SDL_CreateTextureFromSurface(life_gui->renderer, tileset_image);
    SDL_FreeSurface(tileset_image);
    if (life_gui->tileset == NULL) {
        log_sdl_error("SDL_CreateTextureFromSurface");
        destroy_life_gui(life_gui);
        return NULL;
    }

    life_gui->tiles_v = (SDL_Rect *) malloc(256 * sizeof(SDL_Rect));
    if (life_gui->tiles_v == NULL) {
        printf("Error: cant allocate tiles_v\n");
        destroy_life_gui(life_gui);
        return NULL;
    }
    for (int x = 0; x < 16; x++) {
        for (int y = 0; y < 16; y++) {
            SDL_Rect tile;
            tile.x = x * 10;
            tile.y = y * 10;
            tile.w = 10;
            tile.h = 10;
            life_gui->tiles_v[x + y * 16] = tile;
        }
    }

    return life_gui;
}


void draw_world(life_gui_t * life_gui, World * world) {
    SDL_RenderClear(life_gui->renderer);
    SDL_Rect * position = (SDL_Rect *) malloc(sizeof(SDL_Rect));
    position->x = 20;
    position->y = 20;
    position->w = 100;
    position->h = 100;
    SDL_RenderCopy(life_gui->renderer, life_gui->tileset, life_gui->tiles_v + 1, position);
    SDL_RenderPresent(life_gui->renderer);
    SDL_Delay(10000);
}


void destroy_life_gui(life_gui_t * life_gui) {
    if (life_gui->window != NULL) {
        SDL_DestroyWindow(life_gui->window);
    }
    if (life_gui->renderer != NULL) {
        SDL_DestroyRenderer(life_gui->renderer);
    }
    if (life_gui->tileset != NULL) {
        SDL_DestroyTexture(life_gui->tileset);
    }
    free(life_gui);
    SDL_Quit();
}
