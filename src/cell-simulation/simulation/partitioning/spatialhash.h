#ifndef SPATIALHASH_H_INCLUDE
#define SPATIALHASH_H_INCLUDE

// Standard includes.
#include <vector>
#include <unordered_map>

// Project includes.
#include "hashnode.h"

/**
 * @brief This class is used to divide the world in to nodes to speed up collision checks.
 */
class SpatialHash
{
public:

    /**
     * @brief The default spatial hash constructor.
     * @param worldRadius = The world radius.
     */
    SpatialHash(const r32 worldRadius);

    /**
     * @brief The default spatial hash destructor.
     */
    ~SpatialHash();

    /**
     * @brief Build the vertex data used to debug the spatial hash.
     * @param array = The vertex array to store the data.
     * @param type = The primitive type to use.
     */
    void buildArray(sf::VertexArray& array, sf::PrimitiveType type);

    /**
     * @brief Remove an entity from the spatial hash.
     * @param entity = The entity to remove.
     */
    void remove(Entity* entity);

    /**
     * @brief Update an entity and the hash node that it exists in.
     * @param entity = The entity to update.
     */
    void update(Entity* entity);

private:

    /**
     * @brief The hash nodes used in the spatial hash.
     */
    std::unordered_map<u64, HashNode*> m_nodes;

    /**
     * @brief Add an entity into a node based on the hash position (does do bound checks)
     * @param position = The node position.
     * @param entity = The entity to add to a node.
     */
    void addToNode(vec2i position, Entity* entity);
};

#endif // SPATIALHASH_H_INCLUDE
