#ifndef ECOSYSTEM_H_INCLUDE
#define ECOSYSTEM_H_INCLUDE

// Standard includes.
#include <vector>

// SFML includes.
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Text.hpp>

// Project includes.
#include "../typedefs.h"
#include "../core/camera.h"

#include "neuralnetwork.h"
#include "entity.h"

#include "genetics/genome.h"
#include "partitioning/spatialhash.h"

class World
{
public:

    /**
     * @brief The default world constructor.
     */
    World();

    /**
     * @brief The default world destructor.
     */
    ~World();

    /**
     * @brief Initialize the world and load any needed content.
     * @return true if sucessful.
     */
    bool initialize();

    /**
     * @brief Destroy any loaded content.
     */
    void destroy();

    /**
     * @brief Update the world.
     * @param dt = Delta time.
     */
    void update(const float dt);

    /**
     * @brief Render the world.
     * @param target = Target to render to.
     */
    void render(sf::RenderTarget& target, Camera& camera, const sf::View& textView);

    /**
     * @brief Generate a random point that is within the world.
     * @return the random point in the world.
     */
    vec2f randomWorldPoint();

    /**
     * @brief Check if a point is in the world.
     * @param point = The point to check.
     * @return true if the point is in the world.
     */
    bool isPointInWorld(vec2f point);

    /**
     * @brief Check if the specified entity is in the world.
     * @param entity = The entity to check.
     * @return true if the entity is in the world.
     */
    bool isEntityInWorld(Entity* entity);

    /**
     * @brief Enable or disable the debug data.
     * @param debug = The new debug state.
     */
    void setDebug(bool debug) { m_debug = debug; }

    /**
     * @brief Get the current debugging state of the world.
     * @return the current debugging state.
     */
    bool getDebug() const { return m_debug; }

    /**
     * @brief Get the worlds current radius.
     * @return the world radius.
     */
    real32 getRadius() const { return m_radius; }

    /**
     * @brief Get the current count of the entities.
     * @return the current entity count.
     */
    uint32 getEntityCount() const { return m_entities.size(); }

    /**
     * @brief Get an entity by the specified index. (undefined behavior for out of range indexes)
     * @param index = The index to get the entity of.
     * @return the entity at that index.
     */
    Entity* getEntity(uint32 index) { return m_entities[index]; }

    /**
     * @brief Get the entity list for the world.
     * @return a reference to the world entity list.
     */
    std::vector<Entity*>& getEntities() { return m_entities; }

    SpatialHash& getSpatialHash() { return m_spatialHash; }

    /**
     * @brief Add an enitity into the world.
     * @param entity = The entity to add into the world.
     */
    void add(Entity* entity);

    //TODO (Tyler): Clean this up.
    static NeuralNetwork* m_neuralNetwork;
    static uint32 m_weightCount;

private:

    /**
     * @brief Show the debug grid or not.
     */
    bool m_debug;

    /**
     * @brief The radius of the world.
     */
    real32 m_radius;

    /**
     * @brief Used to render the border of the world.
     */
    sf::CircleShape m_border;

    /**
     * @brief The vertex array used for debug rendering.
     */
    sf::VertexArray m_vertexQuadArray;

    /**
     * @brief The vertex array used for debug rendering.
     */
    sf::VertexArray m_vertexLineArray;

    /**
     * @brief The text used to render some special debug information.
     */
    sf::Text* m_debugText;

    /**
     * @brief A list of all the active entities in the world.
     */
    std::vector<Entity*> m_entities;

    /**
     * @brief The spatial hash used to speed up collision checks.
     */
    SpatialHash m_spatialHash;

    /**
     * @brief Occurs when an entity dies in the world.
     * @param entity = The entity that dies.
     */
    void onDeath(Entity* entity);

    /**
     * @brief Update the entity debug text label.
     */
    void updateEntityText();
};

#endif // ECOSYSTEM_H_INCLUDE
