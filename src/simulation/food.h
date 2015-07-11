#ifndef FOOD_H_INCLUDE
#define FOOD_H_INCLUDE

// Project includes.
#include "resource.h"

/**
 * @brief This class represents a food resource in the world.
 */
class Food : public Resource
{
public:

    /**
     * @brief The default food constructor.
     * @param location = The location of the food resource.
     * @param world = The world the resource exists in.
     */
    Food(vec2f location, World& world);
};

#endif // FOOD_H_INCLUDE
