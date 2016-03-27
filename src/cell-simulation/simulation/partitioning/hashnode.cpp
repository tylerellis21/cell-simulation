#include "hashnode.h"
#include "hashutils.h"

bool inList(Entity* e, std::vector<Entity*>& list)
{
    for (auto& entity : list)
        if (entity->getId() == e->getId())
            return true;

    return false;
}

HashNode::HashNode(i32 x, i32 y) :
    m_x(x),
    m_y(y),
    m_hash(hash(x, y))
{
    m_bounds.x = x * cell_size;
    m_bounds.y = y * cell_size;
    m_bounds.width = cell_size;
    m_bounds.height = cell_size;

    m_entities.reserve(10);
}

void HashNode::add(Entity* entity)
{
    // Dont add a null entity.
    if (entity == 0) {
        return;
    }

    m_entities.push_back(entity);
    entity->m_hashNodes.push_back(this);
}

void HashNode::remove(Entity* entity)
{
    // Don't remove a null entity.
    if (entity == 0) {
        return;
    }

    const u32 size = m_entities.size();
    for (u32 i = 0; i < size; i++) {

        Entity* e = m_entities[i];

        if (e->getId() == entity->getId()) {

            m_entities.erase(m_entities.begin() + i);
            break;
        }
    }
}

void HashNode::query(Entity* self, std::vector<Entity*>& list)
{
    if (m_entities.size() == 0)
        return;

    for (auto& entity : m_entities) {

        // Make sure we don't add the same entity more than once.
        if (!inList(entity, list)) {

            // Make sure the list doesn't contain the calling entity.
            if (self->getId() != entity->getId())
                list.push_back(entity);
        }
    }
}

void HashNode::putBounds(sf::VertexArray& array, sf::PrimitiveType type)
{
    r32 lX = m_x * cell_size;
    r32 lY = m_y * cell_size;

    sf::Vector2f a(lX, lY);
    sf::Vector2f b(lX + cell_size, lY);
    sf::Vector2f c(lX + cell_size, lY + cell_size);
    sf::Vector2f d(lX, lY + cell_size);

    sf::Color color = sf::Color(32, 32, 32);

    if (m_entities.size() > 0) {
        color = sf::Color(128, 128, 128);
    }

    if (type == sf::Lines) {
        color = sf::Color::Blue;
        array.append(sf::Vertex(a, color));
        array.append(sf::Vertex(b, color));

        array.append(sf::Vertex(b, color));
        array.append(sf::Vertex(c, color));

        array.append(sf::Vertex(c, color));
        array.append(sf::Vertex(d, color));

        array.append(sf::Vertex(d, color));
        array.append(sf::Vertex(a, color));
    }
    else if (type == sf::Quads) {
        array.append(sf::Vertex(a, color));
        array.append(sf::Vertex(b, color));
        array.append(sf::Vertex(c, color));
        array.append(sf::Vertex(d, color));
    }
}
