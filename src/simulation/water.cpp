#include "water.h"
#include "../util/mathutils.h"

Water::Water(vec2f location, World& world) :
    Resource(randomFloat(50.0f, 100.0f), location, world, type::Water)
{
    m_radius = 4.0f;
    m_mass = 4.0f;
    m_shape.setFillColor(sf::Color(32, 64, 255));
    m_shape.setOutlineColor(sf::Color::Black);
    m_shape.setOutlineThickness(0.2f);
    m_shape.setPosition(m_location.x, m_location.y);
    m_shape.setRadius(m_radius);
    m_shape.setOrigin(m_radius, m_radius);
}
