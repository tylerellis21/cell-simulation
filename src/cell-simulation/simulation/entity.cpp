#include "entity.h"
#include "../core/content.h"
#include "world.h"
#include "partitioning/hashnode.h"
#include "partitioning/hashutils.h"

#include <nex/math/circle.h>
#include <iostream>

// Start the global id counter at zero.
uint32 Entity::m_globalIdCounter = 0;

Entity::Entity(vec2f location, World& world, type::EntityType type) :
    m_id(m_globalIdCounter++),
    m_type(type),
    m_alive(true),
    m_hashUpdate(true),
    m_radius(16.0f),
    m_rotation(0.0f),
    m_mass(1.0f),
    m_location(location),
    m_lastNode(-10000, -10000),
    m_velocity(vec2f()),
    m_friction(vec2f(1.0f)),
    m_world(world),
    m_currentNode(0),
    m_color(0.f)
{ }

Entity::~Entity()
{ }

void Entity::update(const float dt)
{
    // Add the velocity to the location of the entity.
    m_location += m_velocity * dt;

    // Apply the friction to our velocity.
    m_velocity *= m_friction;

    // Update the position of our shape.
    m_shape.setPosition(m_location.x, m_location.y);

    // Only handle the collison once per entity pair.
    std::vector<Entity*> collisionList = getNearEntities(false);

    for (auto& entity : collisionList) {

        if (nx::Circle::intersects(
                    entity->m_location, entity->m_radius,
                    m_location, m_radius)) {

            handleCollision(entity, this);
            onCollision(entity);
        }

    }

    const vec2f worldCenter = vec2f();

    if (!nx::Circle::intersects(
             worldCenter, m_world.getRadius() - m_radius * 2.0f,
             m_location, m_radius)) {

        vec2f normal = vec2f::normalize(worldCenter - m_location);

        m_velocity = m_velocity - (normal * 2.0f * vec2f::dot(normal, m_velocity));

        // v=v-normal*2*dot(normal, v)
        //where normal is the map edge normal (direction from hit point to map origin)

        //TODO (Tyler): Figure out a better method for applying this offset.

        m_location += (m_velocity * dt) * 2.0f;
    }

    vec2i currentNode = calculateNode(m_location);

    m_hashUpdate = (currentNode != m_lastNode);
    m_lastNode = currentNode;
}

/*
 * v1 and v2 are the output velocity.
 * u1 and u2 are the inital velocity.
 *
 * v1 = (u1 * (m1 - m2) * (2 * m2 * u2)) / (m1 + m2)
 */
void Entity::handleCollision(Entity* a, Entity* b)
{
    vec2f u1 = a->m_velocity;
    vec2f u2 = b->m_velocity;

    if (vec2f::dot(u1 - u2, a->getLocation() - b->getLocation()) > 0.0f) {
        return;
    }

    real32 m1 = a->m_mass;
    real32 m2 = b->m_mass;
    real32 sum = m1 + m2;

    a->m_velocity = (u1 * (m1 - m2) + (2.0f * m2 * u2)) / sum;
    b->m_velocity = (u2 * (m2 - m1) + (2.0f * m1 * u1)) / sum;
}

void Entity::render(sf::RenderTarget& target)
{
    target.draw(m_shape, Content::shader);
}

//Also if you want to further optimize you can try pass the vector from outside,

std::vector<Entity*> Entity::getNearEntities(bool fullSearch)
{
    std::vector<Entity*> closest;
    closest.reserve(25);

    if (fullSearch) {

        for (auto& node : m_hashNodes) {
            node->query(this, closest);
        }
    }
    else {

        if (m_currentNode)
            m_currentNode->query(this, closest);
    }

    return closest;
}
