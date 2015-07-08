#ifndef CELL_H_INCLUDE
#define CELL_H_INCLUDE

// Project includes.
#include "entity.h"
#include "genetics/genome.h"
#include "resource.h"

class Cell : public Entity
{
public:

    /**
     * @brief The default cell constructor.
     * @param genome = The genome data for the cell.
     * @param location = The location of the cell.
     * @param world = The world to place the cell in.
     */
    Cell(Genome&& genome, vec2f location, World& world);

    /**
     * @brief Occurs when the cell entity is update.
     * @param dt = Delta time.
     */
    void update(const float dt);

    /**
     * @brief Occurs when the cell entity is rendered.
     * @param target = The target to render to.
     */
    void render(sf::RenderTarget& target);

private:

    /**
     * @brief m_foodAmount
     */
    real32 m_foodAmount;

    /**
     * @brief m_waterAmount
     */
    real32 m_waterAmount;

    /**
     * @brief The genome data for this cell.
     */
    Genome m_genome;

    /**
     * @brief Used to draw the direction line of the cell.
     */
    sf::VertexArray m_directionLine;

    /**
     * @brief Used to draw the food health bar.
     */
    sf::VertexArray m_foodBar;

    /**
     * @brief Used to draw the water health bar.
     */
    sf::VertexArray m_waterBar;

    /**
     * @brief Calculate the vertex data used to render the round info bar.
     */
    void calculateRoundBar(sf::VertexArray& vertexArray, const sf::Color color, const real32 value, const real32 offset);

    /**
     * @brief Occurs when we collide with another entity.
     * @param other = The entity that we collided with.
     */
    void onCollision(Entity *other);

    /**
     * @brief Caculate the closest cell entity and the associated values.
     * @param list = The entity list to use.
     * @param distance = The distance to the cell entity.
     * @param direction = The direction to the cell entity.
     */
    void calculateClosestCell(std::vector<Entity*> list, real32& distance, real32& direction, real32& radius);

    /**
     * @brief Calculate the closest resource entity and the associated values.
     * @param list = The entity list to use.
     * @param distance = The distance to the resource entity.
     * @param direction = The direction toe the resource entity.
     * @param resourceType = The type of resource to look for.
     */
    void calculateClosestResource(std::vector<Entity*> list, real32& distance, real32& direction, type::ResourceType resourceType);

    /**
     * @brief Calculate the verteices for the direction line.
     */
    void calculateDirectionLine();
};

#endif // CELL_H_INCLUDE
