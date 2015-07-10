#include "mathutils.h"

#include <cstdlib>
#include <cstring>
#include <cmath>

bool intersects(const vec2f& circle, const real32 radius, const vec2f& min, const vec2f& max)
{
    const vec2f closest = vec2f::clamp(circle, min, max);
    const vec2f delta = circle - closest;

    const float distanceSquared = (delta.x * delta.x) + (delta.y * delta.y);
    const float radiusSquared = radius * radius;

    return distanceSquared < radiusSquared;
}

bool intersects(const vec2f& circleA, const real32 radiusA, const vec2f& circleB, const real32 radiusB)
{
    const vec2f distance = circleA - circleB;
    const real32 distanceSq = vec2f::dot(distance, distance);

    return !(distanceSq > ((radiusA + radiusB) * (radiusA + radiusB)));
}

real32 randomFloat(real32 min, real32 max)
{
    const real32 range = (max-min);
    return min + (range * rand() / (RAND_MAX + 1.0f));
}

vec2f closestCirclePoint(const vec2f& center, const real32 radius, const vec2f& point)
{
    const vec2f v = point - center;
    const real32 len = 1.0f / v.length();

    return vec2f(center.x + (v.x * len) * radius, center.y + (v.y * len) * radius);
}

real32 normalize(real32 value, const real32 min, const real32 max)
{
    return (value - min) / (max - min);
}

//TODO: (Tyler) Convert this into the proper std::pointer

real32** createMatrix(const uint32 width, const uint32 height)
{
    real32** matrix = new real32*[width];

    for (uint32 x = 0; x < width; x++)
    {
        matrix[x] = new real32[height];
    }

    for (uint32 x = 0; x < width; x++)
    {
        std::memset(matrix[x], 0, height);
    }

    return matrix;
}

void deleteMatrix(real32** matrix, const uint32 width)
{
    for (int x = 0; x < width; x++)
    {
        delete [] matrix[x];
    }

    delete [] matrix;
}
