#include "hashutils.h"

u64 hash(i32 x, i32 y)
{
    return ((u64)x << 32) | ((u64)y & 0xffffffff);
}

vec2i calculateNode(vec2f location)
{
    const i32 x = i32(location.x);

    // This constant is the log base 2 of the cell_size.
    // The cell size also should be a power of two as it currently stands.

    const i32 nodeX = x >> 6; // was 8

    const i32 y = i32(location.y);
    const i32 nodeY = y >> 6;

    return vec2i(nodeX, nodeY);
}
