#include "food.h"
#include "../mathutils.h"
#include "randomgen.h"

Food::Food(vec2f location, World& world) :
    Resource(RandomGen::randomFloat(50.0f, 100.0f), location, world, type::Food)
{
    m_mass = (m_amount / 100.0f) * 50.0f;
    m_shape.setFillColor(sf::Color(25, 255, 25));
    m_shape.setPosition(m_location.x, m_location.y);
    m_shape.setPointCount(32);
    m_shape.setOutlineThickness(0.0f);
    m_color = vec3f(0.f, 255.f, 0.f) / 255.0f;
}
