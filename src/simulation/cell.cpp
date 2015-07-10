#include "cell.h"
#include "world.h"
#include "resource.h"
#include "genetics/breeder.h"

#include "../core/console.h"
#include "../core/content.h"
#include "../util/mathutils.h"

#include <nex/math/mathhelper.h>

#include <iostream>
#include <limits>
#include <sstream>

float IntegerNoise (int n)
{
  n = (n >> 13) ^ n;
  int nn = (n * (n * n * 60493 + 19990303) + 1376312589) & 0x7fffffff;
  return 1.0f - ((float)nn / 1073741824.0f);
}

int32 Cell::m_cellCount = 0;

Cell::Cell(int32 generation, Genome&& genome, vec2f location, World& world) :
    Entity(location, world, type::Cell),
    m_generation(generation),
    m_foodAmount(100.0f),
    m_genome(genome)
{
    m_cellCount++;
    m_mass = 25.0f;
    m_radius = 25.0f;
    m_friction = vec2f(0.98f);

    m_shape.setPosition(m_location.x, m_location.y);
    m_shape.setRadius(m_radius);
    m_shape.setOrigin(m_radius, m_radius);
    m_shape.setPointCount(128);

    m_directionLine.setPrimitiveType(sf::Lines);
    m_foodBar.setPrimitiveType(sf::LinesStrip);


    // The first two values are the split rate, and the mutation rate.
    const real32* genomeData = m_genome.readWeights();

    const int32 red = int32(genomeData[CELL_RED_COLOR_INDEX] * 255.0f);
    const int32 green = int32(genomeData[CELL_GREEN_COLOR_INDEX] * 255.0f);
    const int32 blue = int32(genomeData[CELL_BLUE_COLOR_INDEX] * 255.0f);

    m_shape.setFillColor(sf::Color(red, green, blue, 255));

    m_splitRate = Genome::unscaleSplitRate(genomeData[CELL_SPLIT_RATE_INDEX]);

    m_splitRate = nx::clamp(m_splitRate, 10.0f, 60.0f);

    std::stringstream info;
    info << "cell split rate: " << m_splitRate;
    info << ", mutation rate: " << Genome::unscaleMutationRate(genomeData[CELL_MUTATION_RATE_INDEX]);
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

    // Default food values when none is found.
    real32 foodDist = 0;
    real32 foodDir = 0;

    calculateClosestResource(nearList, foodDist, foodDir, type::Food);

    // Default cell values when none is found.
    real32 cellDist = 0;
    real32 cellDir = 0;
    real32 cellRadius = 0;

    calculateClosestCell(nearList, cellDist, cellDir, cellRadius);

    const vec2f closestWallPoint = closestCirclePoint(vec2f(), m_world.getRadius(), m_location);

    const real32 wallDist = vec2f::distance(closestWallPoint, m_location);
    const real32 wallDir = vec2f::direction(closestWallPoint, m_location);

    //const real32 pi2 = nx::Pi * 2.0f;
    const real32 worldRadius = m_world.getRadius();

    // TODO (Tyler): see if normalized values work better.
    real32 inputs[] = {

        normalize(m_rotation, -nx::Pi, nx::Pi),
        normalize(m_radius, 1.0f, 25.0f),
        normalize(m_foodAmount, 0.0f, 100.0f),

        normalize(foodDist, 0.0f, worldRadius),
        normalize(foodDir, -nx::Pi, nx::Pi),

        normalize(cellDist, 0.0f, worldRadius),
        normalize(cellDir, -nx::Pi, nx::Pi),
        normalize(cellRadius, 1.0f, 25.0f),

        normalize(wallDist, 0.0f, worldRadius),
        normalize(wallDir, -nx::Pi, nx::Pi),

        normalize(IntegerNoise(rand()), -1.0f, 1.0f)
    };

    NeuralNetwork* network = World::m_neuralNetwork;

    // Pass the genome data into the network.

    // We are not using the first two values.
    // Since they are the split rate and the mutation rate.

    network->setWeights(m_genome.readWeights());

    // Compute the output values.
    network->computeOutputs(inputs);

    const real32* output = network->getOutputs();

    const real32 forward = output[0] * 300.0f;
    const real32 backward = output[1] * 300.0f;
    const real32 turnLeft = output[2] * 25.0f;//50.0f;
    const real32 turnRight = output[3] * 25.0f;//50.0f;

    const real32 turnSum = turnLeft - turnRight;
    const real32 moveSum = forward - backward;

    m_rotation += turnSum;

    m_velocity.x += std::cos(m_rotation) * moveSum * dt;
    m_velocity.y += std::sin(m_rotation) * moveSum * dt;

    // Our constant food loss.
    m_foodAmount -= 0.8f * dt;

    // Clamp to the specific range.
    m_foodAmount = nx::clamp(m_foodAmount, 0.0f, 200.0f);

    // The cell considered dead when it is out of water, or it has left the world.
    if (m_foodAmount < 1.0f)
        m_alive = false;

    m_radius = ((m_foodAmount) / 200.0f) * 30.0f;

    m_shape.setRadius(m_radius);
    m_shape.setOrigin(m_radius, m_radius);

    Entity::update(dt);

    // Recalculate the directional line.
    calculateDirectionLine();

    calculateRoundBar(m_foodBar, sf::Color::Green, m_foodAmount, 2.0f);

    splitCell();
}

void Cell::splitCell()
{
    if (m_cellSplitClock.getElapsedTime().asSeconds() >= m_splitRate) {

        // Make sure the cell has enough resources to split.
        if (m_foodAmount >= 90.0f) {

            const real32 pi2 = nx::Pi * 2.0f;
            const real32 randomRad = randomFloat(0.0f, pi2);

            // Add some padding so the new cell doesn't get stuck to us.
            const real32 diameter = (m_radius * 2.0f) + 10.0f;

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

            Cell* baby = new Cell(m_generation + 1, Breeder::replicate(m_genome), newLocation, m_world);

            // Launch the baby cell away so it has a better chance.
            baby->m_velocity = -(m_velocity + vec2f(randomFloat(), randomFloat())) * 2.0f;

            real32 halfFood = m_foodAmount * 0.5f;
            //real32 halfWater = m_waterAmount * 0.5f;

            baby->m_foodAmount = halfFood;
            //baby->m_waterAmount = halfWater;

            m_foodAmount = halfFood;
            //m_waterAmount = halfWater;

            m_world.add(baby);
        }

        m_cellSplitClock.restart();
    }
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

    real32 length = m_velocity.length();
    vec2f newPoint = (length) * vec2f::normalize(m_velocity);

    sf::Vector2f pointA(m_location.x, m_location.y);
    sf::Vector2f pointB(newPoint.x, newPoint.y);

    m_directionLine.append(sf::Vertex(pointA, sf::Color::Red));
    m_directionLine.append(sf::Vertex(pointA + pointB, sf::Color::Red));
}

void Cell::calculateRoundBar(sf::VertexArray& vertexArray, const sf::Color color, const real32 value, const real32 offset)
{
    //TODO: Calculate this as a triangle fan to give more width to the info bars.
    vertexArray.clear();

    const real32 pi2 = nx::Pi * 2.0f;
    const real32 step = pi2 / 16.0f;

    const real32 stopAt = nx::clamp((value / 200.0f) * pi2, 0.0f, pi2);

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

            ocell->m_foodAmount -= 100.0f;
            m_foodAmount += 100.0f;
        }
        // They are bigger than us
        else {
            ocell->m_foodAmount += 100.0f;
            m_foodAmount -= 100.0f;
        }
    }
    // Do cell to resource collision.
    else if (otherType == type::Resource) {

        Resource* resource = (Resource*)other;
        if (resource->getResourceType() == type::Food) {
            m_foodAmount += resource->consume(100.0f);
        }
        else if (resource->getResourceType() == type::Water) {
            //m_waterAmount += resource->consume(10.0f);
        }
    }
}

void Cell::render(sf::RenderTarget& target)
{
    Entity::render(target);
    target.draw(m_directionLine, Content::shader);
    target.draw(m_foodBar, Content::shader);
    //target.draw(m_waterBar, Content::shader);
}
