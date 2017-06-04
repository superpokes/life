#include "World.h"

#include <cstdlib>
#include <cstring>

World::World(const u32 width, const u32 height) {
    w = width;
    h = height;

//    // Dirty
//    land = (Entity **) std::malloc(width * sizeof(Entity *));
//    *land = (Entity *) std::malloc(width * height * sizeof(Entity));
//    std::memset(*land, INANIMATE, width * height * sizeof(Entity));
//    for (u32 i = 1; i < width; i++) {
//        land[i] = land[i - 1] + height;
//    }

    land = (Entity *) malloc(w * h * sizeof(Entity));
    for (u32 i = 0; i < w * h; i++) {
        land[i] = INANIMATE;
    }
}

World::~World() {
    delete land;
}

u32 World::Width() {
    return w;
}

u32 World::Height() {
    return h;
}

Entity World::GetCreature(s32 x, s32 y) {
    if (x >= 0 && y >= 0 && x < w && y < h) {
        return land[y * w + x];
    }
    return INANIMATE;
}

void World::SetCreature(Entity value, s32 x, s32 y) {
    if (x >= 0 && y >= 0 && x < w && y < h) {
        land[y * w + x] = value;
    }
}

void World::ToggleCreature(s32 x, s32 y) {
    if (x >= 0 && y >= 0 && x < w && y < h) {
        land[y * w + x] = land[y * w + x] == SENTIENT ? INANIMATE : SENTIENT;
    }
}

void World::NextGeneration() {
    Entity * new_land = (Entity *) malloc(w * h * sizeof(Entity));

    for (u32 x = 0; x < w; x++) {
        for (u32 y = 0; y < h; y++) {
            u8 adjacents = AdjacentSentients(x, y);
            if (land[y * w + x] == SENTIENT) {
                if (adjacents == 2 || adjacents == 3) {
                    new_land[y * w + x] = SENTIENT;
                } else {
                    new_land[y * w + x] = INANIMATE;
                }
            } else { // == INANIMATE
                if (adjacents == 3) {
                    new_land[y * w + x] = SENTIENT;
                } else {
                    new_land[y * w + x] = INANIMATE;
                }
            }
        }
    }

    delete land;
    land = new_land;
}

const u32 N_DIRECTIONS = 8;
const s32 x_directions[N_DIRECTIONS] = {0, 0, 1, 1, 1, -1, -1, -1};
const s32 y_directions[N_DIRECTIONS] = {1, -1, 0, 1, -1, 0, 1, -1};

u8 World::AdjacentSentients(s32 x, s32 y) {
    u8 count_adjacent = 0;

    for (u32 i = 0; i < N_DIRECTIONS; i++) {
        s32 tmp_x = x + x_directions[i];
        s32 tmp_y = y + y_directions[i];
        if (GetCreature(tmp_x, tmp_y) == SENTIENT) {
            count_adjacent++;
        }
    }

    return count_adjacent;
}

void World::MoveFocus(s32 x, s32 y) {
    // Move Y
    s32 y_offset = w * y;
    if (y_offset > 0) {
        memcpy(land + y_offset, land, (w * h - y_offset) * sizeof(Entity));
        memset(land, INANIMATE, y_offset * sizeof(Entity));
    } else if (y_offset < 0) {
        memcpy(land, land - y_offset, (w * h + y_offset) * sizeof(Entity));
        memset(land + w * h + y_offset, INANIMATE, (-y_offset) * sizeof(Entity));
    }

    // Move X
    size_t byteAmount = (x > 0 ? w - x : w + x) * sizeof(Entity);
    if (x > 0) {
        for (Entity * row = land; row < land + w * h; row += w) {
            memcpy(row + x, row, byteAmount);
            memset(row, INANIMATE, x * sizeof(Entity));
        }
    } else if (x < 0) {
        for (Entity * row = land; row < land + w * h; row += w) {
            memcpy(row, row - x, byteAmount);
            memset(row + w + x, INANIMATE, (-x) * sizeof(Entity));
        }
    }
}
