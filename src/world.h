#include "stdlib.h"

#ifndef WORLD_H
#define WORLD_H

#define INANIMATE 0
#define SENTIENT 1

/*
 * Makes a living entity die, or revives a dead entity.
 * Args:
 *     - ENTITY (entity): the entity we change
 * Side-effects:
 *     - ENTITY changes its contents.
 */
#define KILL(ENTITY) (ENTITY = ENTITY == SENTIENT ? INANIMATE : SENTIENT)

/*
 * For beauty
 */
typedef char Entity;

/*
 * ⴲ long n winded ⴲ
 * Fields:
 *     - w (size_t): width of the world matrix
 *     - h (size_t): height of the world matrix
 *     - land (Entity *): Fuck ** I Don't Want To Create Pointers To Arrays I Want
 *          All My Memory In The Same Place I Hate The C Language.
 */
typedef struct {
    size_t w;
    size_t h;
    Entity * land;
} World;

/*
 * World constructor (feels like god)
 * Args:
 *     - width & height (size_t): you know well
 */
World * init_world(size_t width, size_t height);

/*
 * Spy on your guys.
 *
 * Returns INANIMATE for every position outside of the land's bounds.
 * Args:
 *     - world (World *): the where
 *     - x (size_t): x axis position of the entity
 *     - y (size_t): y axis position of the entity
 * Return:
 *     (Entity) what's where. Except if it's nowhere. Then default.
 */
Entity get_creature(World * world, size_t x, size_t y);

/*
 * Order your guys around.
 *
 * Should accept bad arguments (except a NULL world)
 * Args:
 *     - world (World *): the where
 *     - x (size_t): x axis position of the entity
 *     - y (size_t): y axis position of the entity
 *     - value (Entity): haha, take that! sucker :@
 * Side-effects:
 *     - Well, that (x, y) guy sure got roasted.
 */
void set_creature(World * world, size_t x, size_t y, Entity value);

/*
 * Fuck with your guys.
 *
 * Should accept bad arguments (except a NULL world)
 * Args:
 *     - world (World *): the where
 *     - x (size_t): x axis position of the entity
 *     - y (size_t): y axis position of the entity
 * Side-effects:
 *     - Well, that (x, y) guy sure got roasted.
 */
void toggle_creature(World * world, size_t x, size_t y);

/*
 * Time does indeed have meaning.
 * Go forward with your games.
 * Args:
 *     - world (World *): please no nullerino
 * Side-effects:
 *     - We're never going back to the way we were.
 * Return:
 *     - 0 if completed correctly, 1 otherwise
 */
int next_generation(World * world);

/*
 * Ꮻ deathconsciousness Ꮻ
 */
void destruct_world(World * world);

#endif /* end of include guard: WORLD_H */
