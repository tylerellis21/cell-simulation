#include "world.h"

#include "../core/console.h"
#include "../core/content.h"
#include "../mathutils.h"
#include "randomgen.h"

#include <util/log.h>

#include "cell.h"
#include "food.h"
#include "fire.h"

#include <sstream>
#include <fstream>

NeuralNetwork* World::m_neuralNetwork = 0;
u32 World::m_weightCount = 0;

/*
 * Simulate like a colony
 * Where each entity has a set purpose.
 * Queen
 * Workes
 */

// 8192.0f
World::World() :
    m_radius(2046.0f),
    m_spatialHash(m_radius),
    m_debug(false)
{
    // TODO (Tyler): Fine tune the hidden nodes.
    m_neuralNetwork = new NeuralNetwork(19, 16, 4);
    m_weightCount = m_neuralNetwork->getWeightCount();
}

World::~World()
{
    if (m_neuralNetwork)
        delete m_neuralNetwork;

    m_neuralNetwork = 0;
}

void World::saveState()
{
    std::ofstream out;
    out.open("../../data/dna.dat", std::ios::out | std::ios::binary);

    if (!out.is_open())
        return;

    u64 entityCount = 0;

    for (Entity* e : m_entities)
        if (e->getType() == EntityType::Cell)
            entityCount++;

    out << entityCount << std::endl;

    for (Entity* e : m_entities)
    if (e->getType() == EntityType::Cell){
        Cell* cell = static_cast<Cell*>(e);
        DNA dna = cell->getDna();

        out << cell->getGeneration() << std::endl;

        out << dna.traits.red << std::endl;
        out << dna.traits.green << std::endl;
        out << dna.traits.blue << std::endl;

        out << dna.traits.eyeLengthA << std::endl;
        out << dna.traits.eyeLengthB << std::endl;
        out << dna.traits.eyeLengthC << std::endl;
        out << dna.traits.eyeOffsetA << std::endl;
        out << dna.traits.eyeOffsetB << std::endl;

        out << dna.traits.mutationRate << std::endl;
        out << dna.traits.splitRate << std::endl;

        const r32* genome = dna.genome.readWeights();

        for (u64 i = 0; i < dna.genome.getLength(); i++)
            out << genome[i] << std::endl;
    }

    out.close();
}

void World::loadState()
{
    std::ifstream in;
    in.open("../../data/dna.dat", std::ios::in | std::ios::binary);

    if (!in.is_open())
        return;

    u64 entityCount = 0;
    in >> entityCount;

    for (u64 i = 0; i < entityCount; i++) {
        DNA dna;

        i32 generation = 0;

        in >> generation;
        in >> dna.traits.red;
        in >> dna.traits.green;
        in >> dna.traits.blue;
        in >> dna.traits.eyeLengthA;
        in >> dna.traits.eyeLengthB;
        in >> dna.traits.eyeLengthC;
        in >> dna.traits.eyeOffsetA;
        in >> dna.traits.eyeOffsetB;

        in >> dna.traits.mutationRate;
        in >> dna.traits.splitRate;;

        r32* genome = dna.genome.editWeights();

        for (u64 i = 0; i < dna.genome.getLength(); i++)
            in >> genome[i];

        m_entities.push_back(new Cell(generation, dna, randomWorldPoint(), *this));
    }

    in.close();
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

    loadState();

	if (m_entities.size() < 50) {
		for (i32 i = 0; i < m_entities.size()-50; i++) {
			Cell* newCell = new Cell(1, DNA(), randomWorldPoint(), *this);
			newCell->setMass(100.0f);
			m_entities.push_back(newCell);
		}
	}

    for (i32 i = 0; i < 50; i++) {
        Fire* newCell = new Fire(randomWorldPoint(), *this);
        newCell->setMass(100.0f);
        m_entities.push_back(newCell);
    }


    for (i32 i = 0; i < 250; i++)
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

    saveState();

    for (auto& entity : m_entities) {
        delete entity;
    }

    m_entities.clear();
}

void World::update(const float dt)
{
    bool hasChanged = false;
    bool entityDied = false;

    for (i32 i = m_entities.size() - 1; i >= 0; i--) {
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
    if (entity->getType() == EntityType::Cell) {

        Cell* cell = (Cell*)entity;
        if (cell) {

            //std::stringstream sb;
            //sb << "entity died at generation: " << cell->getGeneration();

            //Console::write(sb.str());

            if (Cell::m_cellCount <= 10) {
				m_entities.push_back(new Cell(1, DNA(), randomWorldPoint(), *this));
            }
        }

    }
    else if (entity->getType() == EntityType::Resource) {

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
    const r32 theta = RandomGen::randomFloat(0.0f, Pi * 2.0f);
    return vec2f(std::cos(theta) * RandomGen::randomFloat(0.0f, m_radius),
                 std::sin(theta) * RandomGen::randomFloat(0.0f, m_radius));
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
