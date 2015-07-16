#include "world.h"

#include "../core/console.h"
#include "../core/content.h"
#include "../mathutils.h"
#include <util/log.h>

#include "cell.h"
#include "food.h"

#include <sstream>

NeuralNetwork* World::m_neuralNetwork = 0;
uint32 World::m_weightCount = 0;

// 8192.0f
World::World() :
    m_radius(4096.0f),
    m_spatialHash(m_radius),
    m_debug(false)
{
    // TODO (Tyler): Fine tune the hidden nodes.
    m_neuralNetwork = new NeuralNetwork(10, 9, 4);
    m_weightCount = m_neuralNetwork->getWeightCount();
}

World::~World()
{
    if (m_neuralNetwork)
        delete m_neuralNetwork;

    m_neuralNetwork = 0;
}

bool World::initialize()
{
    m_border.setRadius(m_radius);
    m_border.setOrigin(m_radius, m_radius);
    m_border.setFillColor(sf::Color(32, 32, 32, 255));
    m_border.setOutlineColor(sf::Color(128, 128, 128, 255));
    m_border.setOutlineThickness(10.0f);
    m_border.setPointCount(100);

    m_vertexQuadArray.setPrimitiveType(sf::Quads);
    m_vertexLineArray.setPrimitiveType(sf::Lines);

    m_debugText = new sf::Text();
    m_debugText->setFont(*Content::font);
    m_debugText->setPosition(0.0f, 100.0f);
    m_debugText->setCharacterSize(16);

    for (int32 i = 0; i < 100; i++) {
        Cell* newCell = new Cell(1, DNA(), randomWorldPoint(), *this);
        newCell->setMass(100.0f);
        m_entities.push_back(newCell);
    }

    for (int32 i = 0; i < 600; i++)
       m_entities.push_back(new Food(randomWorldPoint(), *this));

    m_spatialHash.buildArray(m_vertexQuadArray, sf::Quads);
    m_spatialHash.buildArray(m_vertexLineArray, sf::Lines);

    updateEntityText();

    return true;
}

void World::destroy()
{
    if (m_debugText)
        delete m_debugText;

    for (auto& entity : m_entities) {
        delete entity;
    }

    m_entities.clear();
}

void World::update(const float dt)
{
    bool hasChanged = false;
    bool entityDied = false;

    for (int32 i = m_entities.size() - 1; i >= 0; i--) {
        Entity* entity = m_entities[i];
        if (entity->isAlive()) {

            entity->update(dt);

            if (entity->updateHash()) {
                m_spatialHash.update(entity);
                hasChanged = true;
            }
        }
        else {

            m_entities.erase(m_entities.begin() + i);
            onDeath(entity);

            // Make sure we remove the entity from the spatialhash and all of the nodes that it exists in.
            // This can cause problems because a node may contain a null reference to an entity which causesa c crash.
            m_spatialHash.remove(entity);

            delete entity;
            entityDied = true;
        }
    }

    if (hasChanged && m_debug) {
        m_vertexQuadArray.clear();
        m_vertexLineArray.clear();
        m_spatialHash.buildArray(m_vertexQuadArray, sf::Quads);
        m_spatialHash.buildArray(m_vertexLineArray, sf::Lines);
    }

    if (entityDied) {
        updateEntityText();
    }
}

void World::updateEntityText()
{
    std::stringstream sb;
    sb << "entity count: " << m_entities.size() << std::endl;
    sb << "cell count: " << Cell::m_cellCount;
    m_debugText->setString(sb.str());
}

void World::onDeath(Entity* entity)
{
    if (entity->getType() == type::Cell) {

        Cell* cell = (Cell*)entity;
        if (cell) {

            //std::stringstream sb;
            //sb << "entity died at generation: " << cell->getGeneration();

            //Console::write(sb.str());

            if (Cell::m_cellCount <= 50) {
                //m_entities.push_back(new Cell(1, DNA(), randomWorldPoint(), *this));
            }
        }

    }
    else if (entity->getType() == type::Resource) {

        Resource* resource = (Resource*)entity;
        if (resource->getResourceType() == type::Food) {
            m_entities.push_back(new Food(randomWorldPoint(), *this));
        }
    }
}

void World::render(sf::RenderTarget& target, Camera& camera, const sf::View& textView)
{
    // Setup the camera view for the world
    camera.render(target);

    target.draw(m_border, Content::shader);

    if (m_debug) {
        target.draw(m_vertexQuadArray, Content::shader);
        target.draw(m_vertexLineArray, Content::shader);
    }

    for (auto& entity : m_entities) {

        if (entity->isAlive()) {
            entity->render(target);
        }
    }

    target.setView(textView);
    target.draw(*m_debugText);
}

vec2f World::randomWorldPoint()
{
    // Generate a random angle between 0 and 2(Pi).
    const real32 theta = randomFloat(0.0f) * nx::Pi * 2.0f;
    return vec2f(std::cos(theta) * randomFloat(0.0f, m_radius),
                 std::sin(theta) * randomFloat(0.0f, m_radius));
}

void World::add(Entity* entity)
{
    // Make sure there is never a null entity added.
    if (entity == 0) {
        return;
    }

    m_entities.push_back(entity);
}

bool World::isPointInWorld(vec2f point)
{
    const vec2f delta = point * point;

    return (delta.x + delta.y <= m_radius * m_radius);
}

bool World::isEntityInWorld(Entity* entity)
{
    // Well a null entity can't exist in the world so ya..
    if (entity == 0) {
        return false;
    }

    return isPointInWorld(entity->getLocation());
}
