#ifndef WATER_H_INCLUDE
#define WATER_H_INCLUDE

// Project includes.
#include "resource.h"

/**
 * @brief A simple water resource.
 */
class Water : public Resource
{
public:
    Water(vec2f location, World& world);
};

#endif // WATER_H_INCLUDE
