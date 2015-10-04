#include "mathutils.h"

#include "simulation/randomgen.h"

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


vec2f closestCirclePoint(const vec2f& center, const real32 radius, const vec2f& point)
{
    const vec2f v = point - center;
    const real32 len = 1.0f / v.length();

    return vec2f(center.x + (v.x * len) * radius, center.y + (v.y * len) * radius);
}
/*
Taking

E is the starting point of the ray,
L is the end point of the ray,
C is the center of sphere you're testing against
r is the radius of that sphere
Compute:
d = L - E ( Direction vector of ray, from start to end )
f = E - C ( Vector from center sphere to ray start )
*/
bool circleLineIntersect(const vec2f& E, const vec2f& L, const vec2f& C, const real32 r, real32* distOutput)
{
    const vec2f d = (L - E); // Normalize?
    const vec2f f = E - C;

    real32 a = d.dot(d);
    real32 b = 2.0f * f.dot(d);
    real32 c = f.dot(f) - (r * r);

    real32 discriminant = (b * b) - (4.f * a * c);

    if (discriminant < 0)
        return false;
    else {
        // ray didn't totally miss sphere,
        // so there is a solution to
        // the equation.

        discriminant = sqrt(discriminant);

        // either solution may be on or off the ray so need to test both
        // t1 is always the smaller value, because BOTH discriminant and
        // a are nonnegative.
        real32 t1 = (-b - discriminant)/(2*a);
        real32 t2 = (-b + discriminant)/(2*a);

        // 3x HIT cases:
        //          -o->             --|-->  |            |  --|->
        // Impale(t1 hit,t2 hit), Poke(t1 hit,t2>1), ExitWound(t1<0, t2 hit),

        // 3x MISS cases:
        //       ->  o                     o ->              | -> |
        // FallShort (t1>1,t2>1), Past (t1<0,t2<0), CompletelyInside(t1<0, t2>1)

        if(t1 >= 0 && t1 <= 1) {
            // t1 is the intersection, and it's closer than t2
            // (since t1 uses -b - discriminant)
            // Impale, Poke

            if (distOutput)
                *distOutput = t1;

            return true;
        }

        // here t1 didn't intersect so we are either started
        // inside the sphere or completely past it
        if (distOutput)
            *distOutput = t1;
        return (t2 >= 0 && t2 <= 1);
    }
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
