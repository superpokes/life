#pragma once

#include "Types.h"

enum Entity : unsigned char { SENTIENT, INANIMATE };

// ⴲ
class World {
public:
	World(const u32 width, const u32 height);
	// Ꮻ deathconsciousness Ꮻ
	virtual ~World();
	u32 Width();
	u32 Height();
	// Spy on your guys. 0-indexed. When out of bounds, return inanimate.
	Entity GetCreature(s32 x, s32 y);
	// Order your guys around. 0-indexed. When out of bounds, does nothing.
	void SetCreature(Entity value, s32 x, s32 y);
	// fuck them up. useless if out of bounds.
	void ToggleCreature(s32 x, s32 y);
	void NextGeneration();
	void MoveFocus(s32 x, s32 y);
private:
	u32 w;
	u32 h;
	Entity * land;

	u8 AdjacentSentients(s32 x, s32 y);
};
