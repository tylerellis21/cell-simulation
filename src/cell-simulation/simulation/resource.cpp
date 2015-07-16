#include "resource.h"
#include <nex/math/mathhelper.h>

Resource::Resource(real32 max, vec2f location, World& world, type::ResourceType type) :
    Entity(location, world, type::Resource),
    m_max(max),
    m_amount(max),
    m_resourceType(type)
{
    m_shape.setPosition(location.x, location.y);
    m_friction = vec2f(0.98f);
}

real32 Resource::consume(real32 amount)
{
    real32 toEat = nx::clamp(amount, 0.0f, m_amount);
    m_amount -= toEat;
    return toEat;
}

void Resource::update(const float dt)
{
    // Scale the radius to the amount of resource left.
    m_radius = (m_amount / m_max) * 16.0f;

    // Update the shape to reflect the new radius.
    m_shape.setRadius(m_radius);
    m_shape.setOrigin(m_radius, m_radius);

    //m_amount += 0.01f * dt;

    // Don't let the resource over regenerate.
    if (m_amount > m_max) {
        m_amount = m_max;
    }

    // Die when out of resource.
    if (m_amount < 1.0f) {
        m_alive = false;
    }

    Entity::update(dt);
}
