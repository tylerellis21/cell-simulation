#include "fire.h"
#include "../mathutils.h"
#include "randomgen.h"

Fire::Fire(vec2f location, World& world) :
    Resource(RandomGen::randomFloat(50.0f, 100.0f), location, world, type::Fire)
{
    m_mass = (m_amount / 100.0f) * 50.0f;
    m_friction = {0.99f, 0.99f};
    m_shape.setFillColor(sf::Color::Red);
    m_shape.setPosition(m_location.x, m_location.y);
    m_shape.setPointCount(32);
    m_shape.setOutlineThickness(0.0f);
    m_color = vec3f(128, 128.f, 128.f) / 255.0f;
    m_amount = 100.0f;
}


void Fire::update(const float dt)
{
    dtCounter += dt;
    if (dtCounter >= 1.0f) {
        dtCounter = 0;
        m_velocity.x += RandomGen::randomFloat(-1.f, 1.f) * 10.0f;
        m_velocity.y += RandomGen::randomFloat(-1.f, 1.f) * 10.0f;
    }

    Resource::update(dt);
}
