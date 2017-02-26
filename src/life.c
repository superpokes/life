#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"

#include "world.h"
#include "life_gui.h"

int main(int argc, char const * argv[])
{
    /*
     * Hasta que no generalices un poquito mas las cosas, intenta poner world_w
     * y world_h mayores o iguales que 40 y 30
     */
    int world_w = 50;
    int world_h = 40;
    int tilesize = 20;

    World * world = init_world(world_w, world_h);
    if (world == NULL) {
        printf("Error: init_world.\n");
        return 1;
    }

    LifeGUI * life_gui = init_life_gui(800, 600, world_w, world_h);
    if (life_gui == NULL) {
        printf("Error: init_life_gui.\n");
        destroy_life_gui(life_gui);
        return 1;
    }

    /*
     * test
     */
    toggle_creature(world, 1, 0);
    toggle_creature(world, 2, 1);
    toggle_creature(world, 0, 2);
    toggle_creature(world, 1, 2);
    toggle_creature(world, 2, 2);

    bool quit = false;
    SDL_Event event;
    draw_world(life_gui, world);
    int countTime = 0;
    while (!quit) {
        if (countTime >= 200) {
            next_generation(world);
            draw_world(life_gui, world);
            countTime -= 200;
        }

        if (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_q:
                            quit = true;
                            break;
                        case SDLK_LEFT:
                            move_viewport(life_gui, -1, 0);
                            break;
                        case SDLK_RIGHT:
                            move_viewport(life_gui, 1, 0);
                            break;
                        case SDLK_UP:
                            move_viewport(life_gui, 0, 1);
                            break;
                        case SDLK_DOWN:
                            move_viewport(life_gui, 0, -1);
                            break;
                    }
                    break;
            }
        }

        SDL_Delay(17);
        countTime += 17;
    }

    destroy_life_gui(life_gui);
    destruct_world(world);
    SDL_Quit();
    return 0;
}
