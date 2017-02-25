#include "stdlib.h"
#include "stdio.h"

#include "world.h"
#include "life_gui.h"

#ifdef __MINGW32__
#define EXIT_FUNCTION
#include "conio.h"
bool check_exit()
{
    if (kbhit() && getch() == 27) {
        return true;
    }
    return false;
}
#endif /* __MINGW32__*/

// #ifdef __linux__
// Todo: check_exit()
// #endif /* __linux__ */

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

    for (int i = 0; i < 300; i++) {
        next_generation(world);
        draw_world(life_gui, world);
        #ifdef EXIT_FUNCTION
        if (check_exit()) {
            break;
        }
        #endif
        SDL_Delay(100);
    }

    destroy_life_gui(life_gui);
    destruct_world(world);
    SDL_Quit();
    return 0;
}
