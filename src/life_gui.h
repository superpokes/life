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
 *     - world_w, world_h: useful for not letting the viewport go too far
 */
typedef struct {
    SDL_Window * window;
    SDL_Renderer * renderer;
    SDL_Rect * viewport;
    int _world_h;
    int _world_w;
    int c_x;
    int c_y;
} LifeGUI;

/*
 *
 */
LifeGUI * init_life_gui(int window_w, int window_h, int world_w, int world_h);

/*
 *
 */
void draw_world(LifeGUI * life_gui, World * world);

/*
 *
 */
void move_viewport(LifeGUI * lg, int x, int y);

/*
 *
 */
void destroy_life_gui(LifeGUI * life_gui);

#endif /* end of include guard: LIFE_GUI_H */
