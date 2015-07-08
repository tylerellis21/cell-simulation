#include "cell.h"
#include "world.h"
#include "resource.h"

#include "../core/content.h"
#include "../util/mathutils.h"

#include <nex/math/mathhelper.h>

#include <iostream>
#include <limits>

Cell::Cell(Genome&& genome, vec2f location, World& world) :
    Entity(location, world, type::Cell),
    m_foodAmount(100.0f),
    m_waterAmount(100.0f),
    m_genome(genome)
{
    m_mass = 25.0f;
    m_radius = 25.0f;
    m_friction = vec2f(0.98f);
    m_shape.setFillColor(sf::Color::White);
    m_shape.setOutlineColor(sf::Color::Cyan);
    m_shape.setOutlineThickness(0.2f);
    m_shape.setPosition(m_location.x, m_location.y);
    m_shape.setRadius(m_radius);
    m_shape.setOrigin(m_radius, m_radius);
    m_shape.setPointCount(128);

    m_directionLine.setPrimitiveType(sf::Lines);
    m_foodBar.setPrimitiveType(sf::LinesStrip);
    m_waterBar.setPrimitiveType(sf::LinesStrip);
}

void Cell::update(const float dt)
{
    //m_velocity.x += 5.0f;
    //m_velocity.y += 5.0f;

    // TODO: Add bars to display this instead?
    //real32 redScale = (m_foodAmount / 100.0f) * 255.0f;
    //real32 blueScale = (m_waterAmount / 100.0f) * 255.0f;
    //m_shape.setFillColor(sf::Color(int32(redScale), 0, int32(blueScale)));

    std::vector<Entity*> nearList = getNearEntities();

    // Default food values when none is found.
    real32 foodDist = 0;
    real32 foodDir = 0;

    calculateClosestResource(nearList, foodDist, foodDir, type::Food);

    // Default water values when none is found.
    real32 waterDist = 0;
    real32 waterDir = 0;

    calculateClosestResource(nearList, waterDist, waterDir, type::Water);

    // Default cell values when none is found.
    real32 cellDist = 0;
    real32 cellDir = 0;
    real32 cellRadius = 0;

    calculateClosestCell(nearList, cellDist, cellDir, cellRadius);

    const vec2f closestWallPoint = closestCirclePoint(vec2f(), m_world.getRadius(), m_location);

    const real32 wallDist = vec2f::distance(closestWallPoint, m_location);
    const real32 wallDir = vec2f::direction(closestWallPoint, m_location);

    const real32 pi2 = nx::Pi * 2.0f;
    const real32 worldRadius = m_world.getRadius();

    // TODO (Tyler): see if normalized values work better.
    real32 inputs[] = {

        normalize(m_rotation, -pi2, pi2),

        normalize(m_radius, 1.0f, 100.0f),

        normalize(m_foodAmount, 0.0f, 100.0f),
        normalize(m_waterAmount, 0.0f, 100.0f),

        normalize(foodDist, 0.0f, worldRadius),
        normalize(foodDir, -pi2, pi2),

        normalize(waterDist, 0.0f, worldRadius),
        normalize(waterDir, -pi2, pi2),

        normalize(cellDist, 0.0f, worldRadius),
        normalize(cellDir, -pi2, pi2),
        normalize(cellRadius, 1.0f, 100.0f),

        normalize(wallDist, 0.0f, worldRadius),
        normalize(wallDir, -pi2, pi2)
    };

    NeuralNetwork* network = World::m_neuralNetwork;

    // Pass the genome data into the network.
    network->setWeights(m_genome.readWeights());

    // Compute the output values.
    network->computeOutputs(inputs);

    const real32* output = network->getOutputs();

    // TODO (Tyler): Fine tune this threshold.
    const real32 threshold = 0.5f;

    const real32 speed = output[0];
    const real32 move = output[1];
    const real32 turn = output[2];

    if (move >= threshold) {

        real32 moveSpeed = speed * 100.0f;
        if (moveSpeed < 0) {
            moveSpeed = -moveSpeed;
        }
        real32 moveCost = std::abs(speed) * 0.1f;

        if (m_foodAmount >= moveCost) {
            m_velocity.x += std::cos(m_rotation) * moveSpeed * dt;
            m_velocity.y += std::sin(m_rotation) * moveSpeed * dt;
            m_foodAmount-= moveCost;
        }
    }

    if (turn >= threshold) {

        real32 moveCost = std::abs(speed) * 0.08f;
        if (m_foodAmount >= moveCost) {
            m_rotation += speed * 0.1f;
        }
    }

    // Maybe change food to energy?
    m_foodAmount += 1.8f * dt;
    m_waterAmount -= 1.5f * dt;

    m_foodAmount = nx::clamp(m_foodAmount, 0.0f, 100.0f);
    m_waterAmount = nx::clamp(m_waterAmount, 0.0f, 100.0f);

    // The cell considered dead when it is out of water, or it has left the world.
    if (m_waterAmount <= 0.0f)
        m_alive = false;

    m_radius = ((m_foodAmount + m_waterAmount) / 200.0f) * 25.0f;

    m_shape.setRadius(m_radius);
    m_shape.setOrigin(m_radius, m_radius);

    Entity::update(dt);

    // Recalculate the directional line.
    calculateDirectionLine();
    calculateRoundBar(m_foodBar, sf::Color::Green, m_foodAmount, 2.0f);
    calculateRoundBar(m_waterBar, sf::Color::Blue, m_waterAmount, 4.0f);
}

void Cell::calculateClosestCell(std::vector<Entity*> list, real32& distance, real32& direction, real32& radius)
{
    Entity* found = 0;
    real32 distSq = std::numeric_limits<real32>::max();

    for (auto& entity : list) {

        if (entity->getType() == type::Cell)
            if (vec2f::distanceSquared(entity->getLocation(), m_location) <= distSq)
                found = entity;
    }

    if (found == 0) {
        distance = 0;
        direction = 0;
        radius = 0;
        return;
    }
    else {
        distance = vec2f::distance(found->getLocation(), m_location);
        direction = vec2f::direction(found->getLocation(), m_location);
        radius = found->getRadius();
    }
}

void Cell::calculateClosestResource(std::vector<Entity*> list, real32& distance, real32& direction, type::ResourceType resourceType)
{
    Entity* found = 0;
    real32 distSq = std::numeric_limits<real32>::max();

    for (auto& entity : list) {

        if (entity->getType() == type::Resource)
            if (((Resource*)entity)->getResourceType() == resourceType)
                if (vec2f::distanceSquared(entity->getLocation(), m_location) <= distSq)
                    found = entity;
    }

    if (found == 0) {
        distance = 0;
        direction = 0;
        return;
    }
    else {
        distance = vec2f::distance(found->getLocation(), m_location);
        direction = vec2f::direction(found->getLocation(), m_location);
    }
}

void Cell::calculateDirectionLine()
{
    m_directionLine.clear();

    sf::Vector2f pointA(m_location.x, m_location.y);
    sf::Vector2f pointB(std::cos(m_rotation) * (m_radius + 5.0f),
                        std::sin(m_rotation) * (m_radius + 5.0f));

    m_directionLine.append(sf::Vertex(pointA, sf::Color::Red));
    m_directionLine.append(sf::Vertex(pointA + pointB, sf::Color::Red));
}

void Cell::calculateRoundBar(sf::VertexArray& vertexArray, const sf::Color color, const real32 value, const real32 offset)
{
    vertexArray.clear();

    const real32 pi2 = nx::Pi * 2.0f;
    const real32 step = pi2 / 360.0f;

    const real32 stopAt = nx::clamp((value / 100.0f) * pi2, 0.0f, pi2);

    for (real32 radians = 0; radians <= stopAt; radians += step) {

        vec2f point = vec2f(std::cos(radians) * (m_radius + offset),
                            std::sin(radians) * (m_radius + offset));

        vertexArray.append(sf::Vertex(
                             sf::Vector2f((point.x + m_location.x) ,
                                          (point.y + m_location.y)), color));
    }
}

void Cell::onCollision(Entity* other)
{
    type::EntityType otherType = other->getType();

    // Do cell to cell collision.
    if (otherType == type::Cell) {

        Cell* ocell = (Cell*)other;

        // We are bigger than them.
        if (ocell->m_radius < m_radius) {

            ocell->m_foodAmount -= 5.0f;
            m_foodAmount += 4.0f;
        }
        // They are bigger than us
        else {
            ocell->m_foodAmount += 5.0f;
            m_foodAmount -= 4.0f;
        }
    }
    // Do cell to resource collision.
    else if (otherType == type::Resource) {

        Resource* resource = (Resource*)other;
        if (resource->getResourceType() == type::Food) {
            m_foodAmount += resource->consume(5.0f);
        }
        else if (resource->getResourceType() == type::Water) {
            m_waterAmount += resource->consume(5.0f);
        }
    }
}

void Cell::render(sf::RenderTarget& target)
{
    Entity::render(target);
    target.draw(m_directionLine, Content::shader);
    target.draw(m_foodBar, Content::shader);
    target.draw(m_waterBar, Content::shader);
}
