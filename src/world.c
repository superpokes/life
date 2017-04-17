#include "world.h"

#include "stdbool.h"
#include "stdio.h"
#include "string.h"

// ** PRIVATE **

#define N_DIRECTIONS 8
const char x_directions[N_DIRECTIONS] = { 0, 0, 1, 1, 1,-1,-1,-1 };
const char y_directions[N_DIRECTIONS] = { 1,-1, 0, 1,-1, 0, 1,-1 };

uint8_t adjacent_sentients(World * world, uint32_t x, uint32_t y)
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

// ** END PRIVATE **

World * init_world(uint32_t width, uint32_t height)
{
    World * world = (World *) malloc(sizeof(World));
    if (world == NULL) {
        return NULL;
    }

    world->w = width;
    world->h = height;

    world->land = (Entity **) malloc(width * sizeof(Entity*));
    if (world->land == NULL) {
        return NULL;
    }

    *(world->land) = (Entity *) malloc(width * height * sizeof(Entity));
    if (*(world->land) == NULL) {
        return NULL;
    }

    memset(*(world->land), INANIMATE, width * height * sizeof(Entity));

    for (uint32_t i = 1; i < width; i++) {
        world->land[i] = world->land[i - 1] + height;
    }

    return world;
}


Entity get_creature(World * world, uint32_t x, uint32_t y)
{
    if (x <= world->w && y <= world->h && x >= 1 && y >= 1) {
        return world->land[x - 1][y - 1];
    }
    return INANIMATE;
}


void set_creature(World * world, uint32_t x, uint32_t y, Entity value)
{
    if (x <= world->w && y <= world->h && x >= 1 && y >= 1) {
        world->land[x - 1][y - 1] = value;
    }
}


void toggle_creature(World * world, uint32_t x, uint32_t y) {
    Entity e = get_creature(world, x, y) == SENTIENT ? INANIMATE : SENTIENT;
    set_creature(world, x, y, e);
}


World * next_generation(World * world)
{
    World * next_w = init_world(world->w, world->h);
    if (next_w == NULL) {
        return NULL;
    }

    for (uint32_t x = 1; x < world->w + 1; x++) {
        for (uint32_t y = 1; y < world->h + 1; y++) {
            uint8_t adjacents = adjacent_sentients(world, x, y);
            if (get_creature(world, x, y) == SENTIENT) {
                if (adjacents == 2 || adjacents == 3) {
                    set_creature(next_w, x, y, SENTIENT);
                } else {
                    set_creature(next_w, x, y, INANIMATE);
                }
            } else { // == INANIMATE
                if (adjacents == 3) {
                    set_creature(next_w, x, y, SENTIENT);
                } else {
                    set_creature(next_w, x, y, INANIMATE);
                }
            }
        }
    }

    destruct_world(world);
    return next_w;
}


void destruct_world(World * world)
{
    free(*(world->land));
    free(world->land);
    free(world);
}
