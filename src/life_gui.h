#ifndef LIFE_GUI_H
#define LIFE_GUI_H

#include "SDL.h"
#include "world.h"

/*
 * Necessary data for rendering the game to the screen
 * Fields:
 *     - window (SDL_Window *): Todo
 *     - renderer (SDL_Renderer *): Todo
 *     - viewport (SDL_Rect *): area of the world that will be shown
 *          (Todo: does it make sense to leave it here? when it also concerns
 *           things in the world. IDK.)
 */
typedef struct {
    SDL_Window * window;
    SDL_Renderer * renderer;
    SDL_Rect * viewport;
} life_gui_t;

/*
 *
 */
life_gui_t * init_life_gui();

/*
 *
 */
void draw_world(life_gui_t * life_gui, World * world);

/*
 *
 */
void destroy_life_gui(life_gui_t * life_gui);

#endif /* end of include guard: LIFE_GUI_H */
