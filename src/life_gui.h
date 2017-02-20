#ifndef LIFE_GUI_H
#define LIFE_GUI_H

#include "SDL.h"

typedef struct {
    SDL_Window * window;
    SDL_Renderer * renderer;
} life_gui_t;

life_gui_t * init_life_gui();

void draw_world(life_gui_t * life_gui, World * world);

void destroy_life_gui(life_gui_t * life_gui);

#endif /* end of include guard: LIFE_GUI_H */
