#include "food.h"
#include "../mathutils.h"

Food::Food(vec2f location, World& world) :
    Resource(randomFloat(50.0f, 100.0f), location, world, type::Food)
{
    m_mass = (m_amount / 100.0f) * 50.0f;
    m_shape.setFillColor(sf::Color(236, 92, 116));
    m_shape.setPosition(m_location.x, m_location.y);
    m_shape.setPointCount(32);
    m_shape.setOutlineThickness(0.0f);
}
