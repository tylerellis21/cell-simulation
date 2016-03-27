#ifndef HASHUTILS_H_INCLUDE
#define HASHUTILS_H_INCLUDE

// SCL includes.
#include <scl/types.h>
#include <scl/math/vec2.h>

/**
 * @brief Hash two integer values into one unsigned 64 bit value.
 * @param x = The x value.
 * @param y = The y value.
 * @return the unique hash for the two integer values.
 */
u64 hash(i32 x, i32 y);

/**
 * @brief Calculate the node coordinates based on the given world location.
 * @param location = The world location to find the node for.
 * @return the node coordinates.
 */
vec2i calculateNode(vec2f location);

#endif // HASHUTILS_H_INCLUDE
