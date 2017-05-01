#pragma once

#include "Types.h"

bool InitGrid();
void Clear();
void PaintTile(u32 x, u32 y, u32 tex_n);
void RenderAll();