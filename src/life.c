#include "world.h"
#include "life_gui.h"

int main(int argc, char const * argv[])
{
    life_gui_t * stuff = init_life_gui(1000, 800);
    if (stuff == NULL) {
        destroy_life_gui(stuff);
        return 1;
    }
    draw_world(stuff, NULL);
    destroy_life_gui(stuff);
    return 0;
}
