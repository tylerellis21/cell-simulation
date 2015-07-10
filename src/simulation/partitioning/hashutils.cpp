#include "hashutils.h"

uint64 hash(int32 x, int32 y)
{
    return ((uint64)x << 32) | ((uint64)y & 0xffffffff);
}

vec2i calculateNode(vec2f location)
{
    const int32 x = int32(location.x);
    const int32 nodeX = x >> 7; // was 8

    const int32 y = int32(location.y);
    const int32 nodeY = y >> 7;

    return vec2i(nodeX, nodeY);
}
