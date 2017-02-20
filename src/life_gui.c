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

    life_gui->window = SDL_CreateWindow(100, 100, window_width, window_height, SDL_WINDOW_SHOWN);
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

    return life_gui;
}


void draw_world(life_gui_t * life_gui, World * world) {
    // Todo
}


void destroy_life_gui(life_gui_t * life_gui) {
    // free stuff
    SDL_Quit();
}
