#include "hashutils.h"

uint64 hash(int32 x, int32 y)
{
    return ((uint64)x << 32) | ((uint64)y & 0xffffffff);
}

vec2i calculateNode(vec2f location)
{
    const int32 x = int32(location.x);

    // This constant is the log base 2 of the cell_size.
    // The cell size also should be a power of two as it currently stands.

    const int32 nodeX = x >> 6; // was 8

    const int32 y = int32(location.y);
    const int32 nodeY = y >> 6;

    return vec2i(nodeX, nodeY);
}
