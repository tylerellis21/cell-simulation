#ifndef FIRE_H_INCLUDE
#define FIRE_H_INCLUDE

// Project includes.
#include "resource.h"

/**
 * @brief This class represents a food resource in the world.
 */
class Fire : public Resource
{
public:

    /**
     * @brief The default food constructor.
     * @param location = The location of the food resource.
     * @param world = The world the resource exists in.
     */
    Fire(vec2f location, World& world);

    void update(const r32 dt);

private:

    r32 dtCounter = 0;
};

#endif // FIRE_H_INCLUDE
