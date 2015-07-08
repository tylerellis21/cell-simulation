#include "spatialhash.h"
#include "hashutils.h"

SpatialHash::SpatialHash(const real32 worldRadius)
{
    int32 cell_count = (worldRadius / cell_size) + 2;

    for (int32 x = -cell_count; x <= cell_count; x++) {

        for (int32 y = -cell_count; y <= cell_count; y++) {

            HashNode* newNode = new HashNode(x, y);

            m_nodes[newNode->getHash()] = newNode;
        }
    }
}

SpatialHash::~SpatialHash()
{
    for (auto& node : m_nodes) {
        delete node.second;
    }
}

void SpatialHash::buildArray(sf::VertexArray& array, sf::PrimitiveType type)
{
    for (auto& node : m_nodes) {

        if (node.second)
            node.second->putBounds(array, type);
    }
}

void SpatialHash::update(Entity* entity)
{
    for (auto& node : entity->m_hashNodes) {
        node->remove(entity);
    }
    entity->m_hashNodes.clear();

    const vec2i node = calculateNode(entity->getLocation());

    addToNode(node, entity);

    // Check if the node actually exists.

    // TODO: Improve this stuff here.
    addToNode(vec2i(node.x + 1, node.y), entity);
    addToNode(vec2i(node.x - 1, node.y), entity);

    addToNode(vec2i(node.x, node.y + 1), entity);
    addToNode(vec2i(node.x, node.y - 1), entity);

    addToNode(vec2i(node.x + 1, node.y + 1), entity);
    addToNode(vec2i(node.x - 1, node.y - 1), entity);

    addToNode(vec2i(node.x - 1, node.y + 1), entity);
    addToNode(vec2i(node.x + 1, node.y - 1), entity);
}

void SpatialHash::addToNode(vec2i position, Entity* entity)
{
    HashNode* node = m_nodes[hash(position.x, position.y)];
    if (node) {
        node->add(entity);
    }
}

void SpatialHash::remove(Entity* entity)
{
    if (entity == 0) {
        return;
    }

    // Remove the entity from all the nodes that it exists in.
    for (auto& node : entity->m_hashNodes) {
        node->remove(entity);
    }

    // Clear the entitys hash node list.
    entity->m_hashNodes.clear();
}
