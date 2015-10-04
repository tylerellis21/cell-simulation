#ifndef MATHUTILS_H_INCLUDE
#define MATHUTILS_H_INCLUDE

// Nex includes.
#include <nex/math/vec2.h>
#include <nex/math/mathhelper.h>

using nx::vec2f;

/**
 * @brief Check if the specified circle intersects with the given rectangle.
 * @param circle = The center of the circle.
 * @param radius = The radius of the circle.
 * @param min = The min point of the rectangle.
 * @param max = The max point of the rectangle.
 * @return true if the circle intersects or is contained withe the rectangle, false otherwise.
 */
bool intersects(const vec2f& circle, const real32 radius, const vec2f& min, const vec2f& max);

/**
 * @brief Check if two circles intersect.
 * @param circleA = The center of the first circle.
 * @param radiusA = The radius of the first circle.
 * @param circleB = The center of the second circle.
 * @param radiusB = The radius of teh second circle.
 * @return true if the circles intersects.
 */
bool intersects(const vec2f& circleA, const real32 radiusA, const vec2f& circleB, const real32 radiusB);

bool circleLineIntersect(const vec2f& lineA, const vec2f& lineB, const vec2f& circle, const real32 circleRadius);

/**
 * @brief Get the closest point on a circle to the given point/
 * @param center = The center of the circle.
 * @param radius = The radius of the circle.
 * @param point = The point to get the closest circle point to.
 * @return the closest point on the circle.
 */
vec2f closestCirclePoint(const vec2f& center, const real32 radius, const vec2f& point);

real32 normalize(real32 value, const real32 min, const real32 max);

real32** createMatrix(const uint32 width, const uint32 height);

void deleteMatrix(real32** matrix, const uint32 width);

#endif // MATHUTILS_H_INCLUDE
