#ifndef RESOURCE_H_INCLUDE
#define RESOURCE_H_INCLUDE

// Project includes.
#include "entity.h"

namespace type {

/**
 * @brief Describes the type of resource.
 */
enum ResourceType
{
    Food = 0,
    Water = 1,
    Fire = 2,
};

}

/**
 * @brief This class represents a drainable world resource.
 */
class Resource : public Entity
{
public:

    /**
     * @brief Default resource constructor.
     * @param max = The max amount of resource to start with.
     * @param location = The location of the resource in the world.
     * @param world = The world the resource exists in.
     */
    Resource(real32 max, vec2f location, World& world, type::ResourceType type);

    /**
     * @brief Called when the entity is updated.
     * @param dt = The delta time.
     */
    virtual void update(const float dt);

    /**
     * @brief Get the type of resource that this is.
     * @return The resource type.
     */
    type::ResourceType getResourceType() const { return m_resourceType; }

    /**
     * @brief Consume a specific amount of resource.
     * @param amount = The amount to comsume.
     * @return The amount of resource consumed.
     */
    real32 consume(real32 amount);

    /**
     * @brief Get the current amount of resource available.
     * @return The amount of resource available.
     */
    real32 getAmount() const { return m_amount; }

    /**
     * @brief Set the amount of resource available.
     * @param amount = The amount to set.
     */
    void setAmount(real32 amount) { m_amount = amount; }

protected:

    /**
     * @brief The current amount of resource available.
     */
    real32 m_amount;

    /**
     * @brief The max amount of resource value possible.
     */
    real32 m_max;

    /**
     * @brief The type of resource that this is.
     */
    type::ResourceType m_resourceType;
};

#endif // RESOURCE_H_INCLUDE
