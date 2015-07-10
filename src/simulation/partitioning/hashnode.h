#ifndef HASHNODE_H_INCLUDE
#define HASHNODE_H_INCLUDE

// Standard includes.
#include <vector>

// SFML includes.
#include <SFML/Graphics/VertexArray.hpp>

// Nex includes.
#include <nex/math/rect.h>
using nx::rectf;

// Project includes.
#include "../entity.h"

// Adjust this based on the maximum cell size and the radius of the world.
// Kind of a balancing act really.
const int32 cell_size = 128.0f;

class HashNode
{
public:

    /**
     * @brief The default hashnode constructor.
     * @param x = The x location of the node.
     * @param y = The y location of the node.
     */
    HashNode(int32 x, int32 y);

    /**
     * @brief Add an entity into the hash node.
     * @param entity = The entity to add.
     */
    void add(Entity* entity);

    /**
     * @brief Remove an entity from the hash node.
     * @param entity = The entity to remove.
     */
    void remove(Entity* entity);

    /**
     * @brief Query the node for all of the entitys currently in the node.
     * @param list = The list to add the entities to.
     */
    void query(Entity* self, std::vector<Entity*>& list);

    /**
     * @brief Get the x coordinate of the node.
     * @return the x coordinate node.
     */
    int32 getX() const { return m_x; }

    /**
     * @brief Get the y coordinate of the node.
     * @return the y coordinate node.
     */
    int32 getY() const { return m_y; }

    /**
     * @brief Get the hash value for this node.
     * @return the hash value for the node.
     */
    uint64 getHash() const { return m_hash; }

    /**
     * @brief Put the vertex data used to debug the spatial hash.
     * @param array = The vertex array to append our bound vertices to.
     * @param type = The type of primitive we want to generate.
     */
    void putBounds(sf::VertexArray& array, sf::PrimitiveType type);

private:

    /**
     * @brief The x location of the hash node.
     */
    int32 m_x;

    /**
     * @brief The y location of the hash node.
     */
    int32 m_y;

    /**
     * @brief The hash value of the node.
     */
    uint64 m_hash;

    /**
     * @brief The bounds of the hash node.
     */
    rectf m_bounds;

    /**
     * @brief The list of all the entities in the node.
     */
    std::vector<Entity*> m_entities;
};

#endif // HASHNODE_H_INCLUDE
