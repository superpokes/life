#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"

#include "world.h"
#include "life_gui.h"

int main(int argc, char const * argv[])
{
    life_gui_t * life_gui = init_life_gui(800, 600);
    if (life_gui == NULL) {
        printf("Error: init_life_gui.\n");
        destroy_life_gui(life_gui);
        return 1;
    }

    World * world = init_world(40, 30);
    if (world == NULL) {
        printf("Error: init_world.\n");
        return 1;
    }

    life_gui->viewport->x = 0;
    life_gui->viewport->y = 0;
    life_gui->viewport->w = 40;
    life_gui->viewport->h = 30;

    toggle_creature(world, 1, 0);
    toggle_creature(world, 2, 1);
    toggle_creature(world, 0, 2);
    toggle_creature(world, 1, 2);
    toggle_creature(world, 2, 2);
    draw_world(life_gui, world);

    bool quit = false;
    SDL_Event event;
    while (!quit) {
        next_generation(world);
        draw_world(life_gui, world);

        if (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_q) {
                        quit = true;
                    }
                    break;
            }
        }
        
        SDL_Delay(100);
    }

    destroy_life_gui(life_gui);
    destruct_world(world);
    SDL_Quit();
    return 0;
}
