#ifndef ENTITY_H_INCLUDE
#define ENTITY_H_INCLUDE

// Standard includes.

// SFML includes.
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/CircleShape.hpp>

// Nex includes.
#include <nex/math/vec2.h>

using nx::vec2f;
using nx::vec2i;

// Forward reference.
class World;
class HashNode;

namespace type {

enum EntityType
{
    Cell = 0,
    Resource = 1
};

}

class Entity
{
public:
    friend class SpatialHash;
    friend class HashNode;

    /**
     * @brief Construct an entity at the specified location.
     * @param location = The location to place the entitiy.
     * @param world = The world the entity exists in.
     * @param type = The type of entity that is being created.
     */
    Entity(vec2f location, World& world, type::EntityType type);

    /**
     * @brief The default entity destructor.
     */
    virtual ~Entity();

    /**
     * @brief Update the entity.
     * @param dt = The delta time.
     */
    virtual void update(const float dt);

    /**
     * @brief Render the entity.
     * @param target = The target to render to.
     */
    virtual void render(sf::RenderTarget& target);

    /**
     * @brief Get the unique id of the entity.
     * @return the unique id of the entity.
     */
    uint32 getId() const { return m_id; }

    /**
     * @brief Get the type id of this entity.
     * @return the type of entity.
     */
    type::EntityType getType() const { return m_type; }

    /**
     * @brief Get the state of the entity.
     * @return true if the entity is alive.
     */
    bool isAlive() const { return m_alive; }

    /**
     * @brief Update the hash map for this entity.
     * @return true if the entity needs a hash map update.
     */
    bool updateHash() const { return m_hashUpdate; }

    /**
     * @brief Set the state of the entity.
     * @param alive = The state of the entity.
     */
    void setAlive(bool alive) { m_alive = alive; }

    /**
     * @brief Set the mass of the entity.
     * @param mass = The mass to set.
     */
    void setMass(real32 mass) { m_mass = mass; }

    /**
     * @brief Get the current radius of the entity.
     * @return the radius of the entity.
     */
    real32 getRadius() const { return m_radius; }

    /**
     * @brief Get the location of the entity.
     * @return the current entity location.
     */
    vec2f getLocation() const { return m_location; }

    /**
     * @brief Set the velocity of the entity.
     * @param velocity = The velocity to set.
     */
    void setVelocity(vec2f velocity) { m_velocity = velocity; }

    /**
     * @brief Set the radius of the entity.
     * @param radius = The new radius of the entity.
     */
    void setRadius(real32 radius) { m_radius = radius; }

private:

    /**
     * @brief The global id counter, used to assign each entity a unique entity.
     */
    static uint32 m_globalIdCounter;

    /**
     * @brief The unique id of the entity.
     */
    uint32 m_id;

    /**
     * @brief Handle the collision between two entities.
     * @param a = The first entity.
     * @param b = The second entity.
     */
    void handleCollision(Entity* a, Entity* b);

    /**
     * @brief This method gets called when we collided with another entity.
     * @param other = The other entity that we collided with.
     */
    virtual void onCollision(Entity* other) { }

protected:

    /**
     * @brief The type of entity that this is.
     */
    type::EntityType m_type;

    /**
     * @brief The state of the entity.
     */
    bool m_alive;

    /**
     * @brief Triggers the entity to be updated in the spatial hash.
     */
    bool m_hashUpdate;

    /**
     * @brief The radius of our center.
     */
    real32 m_radius;

    /**
     * @brief The current rotation of the entity.
     */
    real32 m_rotation;

    /**
     * @brief The mass of the entity used for collision response.
     */
    real32 m_mass;

    /**
     * @brief The location of the entity.
     */
    vec2f m_location;

    /**
     * @brief The last node the entity was in.
     */
    vec2i m_lastNode;

    /**
     * @brief The current velocity of the entity.
     */
    vec2f m_velocity;

    /**
     * @brief The friction of the entity.
     */
    vec2f m_friction;

    /**
     * @brief The world that the entity exists in.
     */
    World& m_world;

    /**
     * @brief The shape used to draw this entity.
     */
    sf::CircleShape m_shape;

    /**
     * @brief The current node that the entity is in.
     */
    HashNode* m_currentNode;

    /**
     * @brief The list of hash node that the entity exists in.
     */
    std::vector<HashNode*> m_hashNodes;

    /**
     * @brief Calculate the entities that are close to this entity.
     * @return the entities that are closest to this entity.
     */
    std::vector<Entity*> getNearEntities(bool fullSearch);

};

#endif // ENTITY_H_INCLUDE
