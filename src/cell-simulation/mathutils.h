#ifndef MATHUTILS_H_INCLUDE
#define MATHUTILS_H_INCLUDE

// SCL includes.
#include <scl/math/vec2.h>
#include <scl/math/help.h>

bool intersects(const vec2f& circle, const r32 radius, const vec2f& min, const vec2f& max);

bool intersects(const vec2f& circleA, const r32 radiusA, const vec2f& circleB, const r32 radiusB);

bool circleLineIntersect(const vec2f& lineA, const vec2f& lineB, const vec2f& circleCenter, const r32 circleRadius, r32* distOutput = 0);

vec2f closestCirclePoint(const vec2f& center, const r32 radius, const vec2f& point);

r32 normalize(r32 value, const r32 min, const r32 max);

r32** createMatrix(const u32 width, const u32 height);

void deleteMatrix(r32** matrix, const u32 width);

#endif // MATHUTILS_H_INCLUDE
