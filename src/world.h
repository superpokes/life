#include "stdlib.h"

#ifndef WORLD_H
#define WORLD_H

#define INANIMATE 0
#define SENTIENT 1

#define KILL(ENTITY) (ENTITY == SENTIENT ? INANIMATE : SENTIENT)

typedef struct {
    size_t w;
    size_t h;
    char * land;
} World;

World * init_world(size_t width, size_t height);

/*
 * Returns INANIMATE for every position outside of the land's bounds.
 */
char get_creature(World * world, size_t x, size_t y);

/*
 * 0 completed correctly
 * 1 didn't
 */
int next_generation(World * world);

void destruct_world(World * world);

#endif /* end of include guard: WORLD_H */
