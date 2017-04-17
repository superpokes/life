#include "stdlib.h"
#include "stdint.h"

#ifndef WORLD_H
#define WORLD_H

typedef enum { SENTIENT, INANIMATE } Entity;

/*
 * ⴲ 2 long ⴲ
 */
typedef struct {
    uint32_t w;
    uint32_t h;
    Entity ** land;
} World;

/*
 * World constructor (feels like god)
 */
World * init_world(uint32_t width, uint32_t height);

/*
 * Spy on your guys. 1-indexed. When out of bounds, return inanimate.
 */
Entity get_creature(World * world, uint32_t x, uint32_t y);

/*
 * Order your guys around. 1-indexed. When out of bounds, does nothing.
 */
void set_creature(World * world, uint32_t x, uint32_t y, Entity value);

/*
 * fuck them up. useless if out of bounds.
 */
void toggle_creature(World * world, uint32_t x, uint32_t y);

/*
 * If it doesn't generate correctly, return will be a null pointer.
 * world just breaks
 */
World * next_generation(World * world);

/*
 * Ꮻ deathconsciousness Ꮻ
 */
void destruct_world(World * world);

#endif /* end of include guard: WORLD_H */
