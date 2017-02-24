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

    life_gui->viewport = (SDL_Rect *) malloc(sizeof(SDL_Rect));
    if (life_gui->viewport == NULL) {
        printf("Error: cant allocate viewport\n");
        destroy_life_gui(life_gui);
        return NULL;
    }

    return life_gui;
}


void draw_world(life_gui_t * g, World * w)
{
    SDL_RenderClear(g->renderer);
    for (int y = g->viewport->y; y < g->viewport->y + g->viewport->h; y++) {
        for (int x = g->viewport->x; x < g->viewport->x + g->viewport->w; x++) {
            if (get_creature(w, x, y) == SENTIENT) {
                SDL_SetRenderDrawColor(g->renderer, 0, 0, 255, 255); // blue
            } else {
                uint8_t shade = (x + y * g->viewport->w) % 2 ? 245 : 255; // white-ish
                SDL_SetRenderDrawColor(g->renderer, shade, shade, shade, 255);
            }
            SDL_Rect pos;
            pos.x = x * 20;
            pos.y = y * 20;
            pos.w = 20;
            pos.h = 20;
            SDL_RenderFillRect(g->renderer, &pos);
            SDL_SetRenderDrawColor(g->renderer, 0, 0, 0, 255);
            SDL_RenderDrawRect(g->renderer, &pos);
        }
    }
    SDL_RenderPresent(g->renderer);
}


void destroy_life_gui(life_gui_t * life_gui)
{
    if (life_gui->window != NULL) {
        SDL_DestroyWindow(life_gui->window);
    }
    if (life_gui->renderer != NULL) {
        SDL_DestroyRenderer(life_gui->renderer);
    }
    if (life_gui->viewport != NULL) {
        free(life_gui->viewport);
    }
    free(life_gui);
    SDL_Quit();
}
