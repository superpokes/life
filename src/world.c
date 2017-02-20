#include "world.h"

#define N_DIRECTIONS
const char x_directions[N_DIRECTIONS] = { 0, 0, 1, 1, 1,-1,-1,-1 };
const char y_directions[N_DIRECTIONS] = { 1,-1, 0, 1,-1, 0, 1,-1 };

World * init_world(size_t width, size_t height)
{
    World * world = (World *) malloc(sizeof(World));
    if (world == NULL) {
        return NULL;
    }

    world->w = width;
    world->h = height;
    world->land = (char *) malloc(width * height * sizeof(char));
    if (world->land == NULL) {
        return NULL;
    }

    return world;
}


char get_creature(World * world, size_t x, size_t y)
{
    if (x >= world->w || y >= world->h) {
        return INANIMATE;
    } else {
        return world->land[x + y * world->w];
    }
}


void set_creature(World * world, size_t x, size_t y, char value) {
    if (x < world->w && y < world->h) {
        world->land[x + y * world->w] = value;
    }
}


void toggle_creature(World * world, size_t x, size_t y) {
    if (x < world->w && y < world->h) {
        world->land[x + y * world->w] = \
            world->land[x + y * world->w] == SENTIENT ? INANIMATE : SENTIENT;
    }
}


unsigned char adjacent_sentients(World * world, size_t x, size_t y)
{
    int i_x = (int) x;
    int i_y = (int) y;
    int count_adjacent = 0;

    for (int i = 0; i < N_DIRECTIONS; i++) {
        int tmp_x = i_x + x_directions[i];
        int tmp_y = i_y + y_directions[i];
        if (get_creature(world, tmp_x, tmp_y) == SENTIENT) {
            count_adjacent++;
        }
    }

    return count_adjacent;
}


int next_generation(World * world)
{
    char * next_generation_land = (char *) malloc(world->w * world->h * sizeof(char));
    if (next_generation_land == NULL) {
        return 1;
    }

    for (size_t x = 0; x < world->w; x++) {
        for (size_t y = 0; y < world->h; y++) {
            unsigned char adjacents = adjacent_sentients(world, x, y);
            if (get_creature(world, x, y) == SENTIENT) {
                if (adjacents == 2 || adjacents == 3) {
                    next_generation_land[x + y * world->w] = SENTIENT;
                } else {
                    next_generation_land[x + y * world->w] = INANIMATE;
                }
            } else { // == INANIMATE
                if (adjacents == 3) {
                    next_generation_land[x + y * world->w] = SENTIENT;
                } else {
                    next_generation_land[x + y * world->w] = INANIMATE;
                }
            }
        }
    }

    free(world->land);
    world->land = next_generation_land;

    return 0;
}


void destruct_world(World * world)
{
    free(world->land);
    free(world);
}
