#include "cell.h"
#include "world.h"
#include "resource.h"
#include "genetics/breeder.h"

#include "../core/console.h"
#include "../core/content.h"
#include "../mathutils.h"
#include "randomgen.h"

#include <scl/math/help.h>

#include <iostream>
#include <limits>
#include <sstream>

const r32 CELL_MAX_MASS = 100.0f;
const r32 CELL_MAX_FOOD = 100.0f;
const r32 CELL_MAX_RADIUS = 30.0f;

r32 IntegerNoise (i32 n)
{
  n = (n >> 13) ^ n;
  i32 nn = (n * (n * n * 60493 + 19990303) + 1376312589) & 0x7fffffff;
  return 1.0f - ((r32)nn / 1073741824.0f);
}

i32 Cell::m_cellCount = 0;

Cell::Cell(i32 generation, DNA dna, vec2f location, World& world) :
    Entity(location, world, EntityType::Cell),
    m_generation(generation),
    m_foodAmount(CELL_MAX_FOOD),
    m_dna(dna)
{
    m_cellCount++;

    m_friction = vec2f(0.95f);

    m_shape.setPointCount(32);
    m_shape.setPosition(m_location.x, m_location.y);
    m_shape.setOutlineThickness(0.0f);

    m_debugLines.setPrimitiveType(sf::Lines);
    m_foodBar.setPrimitiveType(sf::LinesStrip);

    m_mass = CELL_MAX_MASS;

    m_shape.setFillColor(sf::Color(m_dna.traits.red * 255, m_dna.traits.green * 255, m_dna.traits.blue * 255, 255));

    m_color = vec3f(m_dna.traits.red, m_dna.traits.green, m_dna.traits.blue);

    m_splitRate = m_dna.traits.splitRate;

    std::stringstream info;
    info << "cell split rate: " << m_splitRate;
    info << ", mutation rate: " << m_dna.traits.mutationRate;
    info << ", gen: " << m_generation;

    Console::write(info.str());
}

Cell::~Cell()
{
    m_cellCount--;
}

void Cell::update(const float dt)
{
    time += dt;

    // All of the entities in the nearby hashnodes.
    std::vector<Entity*> nearList = getNearEntities(true);

    const vec2f closestWallPoint = closestCirclePoint(vec2f(), m_world.getRadius(), m_location);
    const r32 wallDist = vec2f::distance(closestWallPoint, m_location);
    const r32 wallDir = m_rotation - vec2f::direction(closestWallPoint, m_location);

    //const r32 pi2 = nx::Pi * 2.0f;
    const r32 worldRadius = m_world.getRadius();

    r32 visionValues[12];

    for (auto& value : visionValues)
        value = 0;

    caculateVisionLines();
    calculateVision(nearList, visionValues);

    r32 inputs[19];

    inputs[0] = normalize(m_rotation, -Pi, Pi);
    inputs[1] = normalize(m_radius, 1.0f, CELL_MAX_RADIUS);
    inputs[2] = normalize(m_foodAmount, 0.0f, CELL_MAX_FOOD);
    inputs[3] = normalize(wallDist, 0.0f, worldRadius);
    inputs[4] = normalize(wallDir, -Pi, Pi);
    inputs[5] = m_memory[0];
    inputs[6] = m_memory[1];

    for (u32 i = 0; i < 12; i++)
        inputs[i + 7] = visionValues[i];

    NeuralNetwork* network = World::m_neuralNetwork;

    // Pass the genome data into the network.

    // We are not using the first two values.
    // Since they are the split rate and the mutation rate.

    network->setWeights(m_dna.genome.readWeights());

    // Compute the output values.
    network->computeOutputs(inputs);

    const r32* output = network->getOutputs();

    const r32 forward = output[0] * 300.f;
    const r32 turnLeft = output[2];
    const r32 turnRight = output[3];


    m_memory[0] = output[4];
    m_memory[1] = output[5];

    m_rotation += turnRight / Pi;
    m_rotation -= turnLeft / Pi;

    m_velocity.x += std::cos(m_rotation) * forward * dt;
    m_velocity.y += std::sin(m_rotation) * forward * dt;

    // Our constant food loss.
    m_foodAmount -= 5.0f * dt;

    // Clamp to the specific range.
    m_foodAmount = clamp(m_foodAmount, 0.0f, CELL_MAX_FOOD);
    m_mass = clamp(m_mass, 1.0f, CELL_MAX_MASS);

    // The cell considered dead when it is out of water, or it has left the world.
    if (m_foodAmount < 1.0f || m_mass < 2.0f)
        m_alive = false;

    m_radius = (m_mass / CELL_MAX_MASS) * CELL_MAX_RADIUS;

    m_shape.setRadius(m_radius);
    m_shape.setOrigin(m_radius, m_radius);

    Entity::update(dt);

    // Recalculate the directional line.
    caculateDebugLines();

    calculateRoundBar(m_foodBar, sf::Color::Green, m_foodAmount, 2.0f);

    splitCell();
}

void Cell::splitCell()
{
    const r32 timePassed = m_cellSplitClock.getElapsedTime().asSeconds();

    if (timePassed >= m_splitRate || (m_foodAmount >= 10.0f && timePassed >= 10.0f)) {

        const r32 pi2 = Pi * 2.0f;
        const r32 randomRad = RandomGen::randomFloat(0.0f, 2.0f * Pi);

        // Add some padding so the new cell doesn't get stuck to us.
        const r32 diameter = (m_radius * 2.0f) + 10.0f;

        int tries = 0;
        vec2f newLocation;
        do
        {
            newLocation = m_location + vec2f(std::cos(randomRad) * diameter,
                                             std::sin(randomRad) * diameter);
            tries++;

            if (tries > 9) {
                newLocation = m_world.randomWorldPoint();
            }
        }
        while(!m_world.isPointInWorld(newLocation) && tries < 10);

        // TODO: Check the breeder and how it is moving/copying genomes.

        Cell* baby = new Cell(m_generation + 1, Breeder::replicate(m_dna), newLocation, m_world);

        const r32 halfMass = m_mass * 0.5f;

        baby->m_mass = halfMass;
        m_mass = halfMass;

        // Take some energy since we just divided.
        m_foodAmount -= 25.0f;

        // Launch the baby cell away so it has a better chance.
        baby->m_velocity = -(m_velocity);

        m_world.add(baby);

        m_cellSplitClock.restart();
    }
}

void Cell::calculateClosestCell(std::vector<Entity*> list, r32& distance, r32& direction, r32 radius)
{
    Entity* found = 0;
    r32 distSq = std::numeric_limits<r32>::max();

    for (auto& entity : list) {

        if (entity->getType() == EntityType::Cell)
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
        direction = m_rotation - vec2f::direction(found->getLocation(), m_location);
        radius = found->getRadius();
    }
}

void Cell::calculateClosestResource(std::vector<Entity*> list, r32& distance, r32& direction, type::ResourceType resourceType)
{
    Entity* found = 0;
    r32 distSq = std::numeric_limits<r32>::max();

    for (auto& entity : list) {

        if (entity->getType() == EntityType::Resource)
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
        direction = m_rotation - vec2f::direction(found->getLocation(), m_location);
    }
}

void Cell::caculateVisionLines()
{
    const r32 rotationA = m_rotation - m_dna.traits.eyeOffsetA;
    const r32 rotationB = m_rotation + m_dna.traits.eyeOffsetB;

    // TODO: Remove the trig functions here.

    m_visionLines[0] = m_location + vec2f(std::cos(m_rotation) * m_dna.traits.eyeLengthA, std::sin(m_rotation) * m_dna.traits.eyeLengthA);
    m_visionLines[1] = m_location + vec2f(std::cos(rotationA) * m_dna.traits.eyeLengthB, std::sin(rotationA) * m_dna.traits.eyeLengthB);
    m_visionLines[2] = m_location + vec2f(std::cos(rotationB) * m_dna.traits.eyeLengthC, std::sin(rotationB) * m_dna.traits.eyeLengthC);
}

void Cell::calculateVision(std::vector<Entity*> list, r32* outputs)
{
    VisionResult results[3];

    for (auto& entity : list) {

        r32 distA = 0;
        r32 distB = 0;
        r32 distC = 0;

        bool intersectA =
                circleLineIntersect(m_location, m_visionLines[0], entity->getLocation(), entity->getRadius(), &distA);

        bool intersectB =
                circleLineIntersect(m_location, m_visionLines[1], entity->getLocation(), entity->getRadius(), &distB);

        bool intersectC =
                circleLineIntersect(m_location, m_visionLines[2], entity->getLocation(), entity->getRadius(), &distC);

        if (intersectA) {
            results[0].color = entity->getColor();
            results[0].distance = distA;
        }

        if (intersectB) {
            results[1].color = entity->getColor();
            results[1].distance = distB;
        }

        if (intersectC) {
            results[2].color = entity->getColor();
            results[2].distance = distC;
        }
    }

    for (u32 offset = 0, i = 0; i < 4; i++, offset += 4) {
        outputs[offset] = results[i].distance;
        outputs[offset+1] = results[i].color.x;
        outputs[offset+2] = results[i].color.y;
        outputs[offset+3] = results[i].color.z;
    }
}

void Cell::caculateDebugLines()
{
    m_debugLines.clear();

    r32 length = (m_velocity.length() / 500.0f) + 8.0f;
    vec2f newPoint = (length) * vec2f::normalizeOrZero(m_velocity);

    sf::Vector2f pointA(m_location.x, m_location.y);
    sf::Vector2f pointB(newPoint.x, newPoint.y);

    m_debugLines.append(sf::Vertex(pointA, sf::Color::Red));
    m_debugLines.append(sf::Vertex(pointA + pointB, sf::Color::Red));

    m_debugLines.append(sf::Vertex(pointA, sf::Color::Cyan));
    m_debugLines.append(sf::Vertex(sf::Vector2f(m_visionLines[0].x, m_visionLines[0].y), sf::Color::Cyan));

    m_debugLines.append(sf::Vertex(pointA, sf::Color::Green));
    m_debugLines.append(sf::Vertex(sf::Vector2f(m_visionLines[1].x, m_visionLines[1].y), sf::Color::Green));

    m_debugLines.append(sf::Vertex(pointA, sf::Color::Blue));
    m_debugLines.append(sf::Vertex(sf::Vector2f(m_visionLines[2].x, m_visionLines[2].y), sf::Color::Blue));
}

void Cell::calculateRoundBar(sf::VertexArray& vertexArray, const sf::Color color, const r32 value, const r32 offset)
{
    vertexArray.clear();

    const r32 pi2 = Pi * 2.0f;
    const r32 step = pi2 / 16.0f;

    const r32 stopAt = clamp((value / CELL_MAX_FOOD) * pi2, 0.0f, pi2);

    for (r32 radians = 0; radians <= stopAt; radians += step) {

        vec2f point = vec2f(std::cos(radians) * (m_radius + offset),
                            std::sin(radians) * (m_radius + offset));

        vertexArray.append(sf::Vertex(
                             sf::Vector2f((point.x + m_location.x) ,
                                          (point.y + m_location.y)), color));
    }
}

void Cell::onCollision(Entity* other)
{
    EntityType otherType = other->getType();

    // Do cell to cell collision.
    if (otherType == EntityType::Cell) {

        Cell* ocell = (Cell*)other;

        // so splitting wouldn't actually make their food level = half because of their new mass
        // Divide the incoming food by the mass of the cell then add it to the food value

        // TODO: add more size ranges on the food

        // We are bigger than them.
        if (ocell->m_mass < m_mass) {

            ocell->m_foodAmount -= 8.0f;
            m_foodAmount += 4.0f;
            m_mass += 4.0f;
        }
        // They are bigger than us
        else {

            ocell->m_foodAmount += 8.0f;
            m_foodAmount -= 4.0f;
            m_mass -= 4.0f;
        }
    }
    // Do cell to resource collision.
    else if (otherType == EntityType::Resource) {

        Resource* resource = (Resource*)other;
        if (resource->getResourceType() == type::Food) {
            m_foodAmount += resource->consume(8.0f);
            m_mass += 4.0f;
        }
        else if (resource->getResourceType() == type::Fire)
            m_foodAmount -= 10.0f;

    }
}

void Cell::render(sf::RenderTarget& target)
{
    Entity::render(target);
    target.draw(m_debugLines, Content::shader);
    target.draw(m_foodBar, Content::shader);
}
