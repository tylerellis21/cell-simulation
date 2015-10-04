#ifndef CELL_H_INCLUDE
#define CELL_H_INCLUDE

#include <nex/math/vec2.h>
#include <nex/math/vec3.h>

// SFML includes.
#include <SFML/System/Clock.hpp>

// Project includes.
#include "entity.h"
#include "genetics/dna.h"
#include "resource.h"

using nx::vec2f;
using nx::vec3f;


/**
 * @brief This class represents a cell in the simulation world.
 */
class Cell : public Entity
{
public:

    /**
     * @brief The default cell constructor.
     * @param dna = The dna data for the cell.
     * @param location = The location of the cell.
     * @param world = The world to place the cell in.
     */
    Cell(int32 generation, DNA dna, vec2f location, World& world);

    ~Cell();

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

    /**
     * @brief The number of active cells in the world.
     */
    static int32 m_cellCount;

    /**
     * @brief Get the generation number of this cell.
     * @return The generation of the cell.
     */
    int32 getGeneration() const { return m_generation; }

private:

    /**
     * @brief The current generation of the cell.
     */
    int32 m_generation;

    /**
     * @brief The amount of food that the cell currently has.
     */
    real32 m_foodAmount;

    /**
     * @brief The time in seconds in at which the cell splits.
     */
    real32 m_splitRate;

    /**
     * @brief The time used to provide a constant source of input.
     */
    real32 time = 0.0f;

    /**
     * @brief The dna data for this cell.
     */
    DNA m_dna;

    struct VisionResult {
        real32 distance = 0.f;
        vec3f color = {0.f, 0.f, 0.f};
    };

    vec2f m_visionLines[6];

    /**
     * @brief Used to draw the direction line of the cell.
     */
    sf::VertexArray m_debugLines;

    /**
     * @brief Used to draw the food health bar.
     */
    sf::VertexArray m_foodBar;

    /**
     * @brief The clock used to track the cell splitting processes.
     */
    sf::Clock m_cellSplitClock;

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
    void calculateClosestCell(std::vector<Entity*> list, real32& distance, real32& direction, real32 radius);

    /**
     * @brief Calculate the closest resource entity and the associated values.
     * @param list = The entity list to use.
     * @param distance = The distance to the resource entity.
     * @param direction = The direction toe the resource entity.
     * @param resourceType = The type of resource to look for.
     */
    void calculateClosestResource(std::vector<Entity*> list, real32& distance, real32& direction, type::ResourceType resourceType);

    void calculateVision(std::vector<Entity*> list, real32* outputs);

    /**
     * @brief Calculate the verteices for the direction line.
     */
    void caculateDebugLines();

    void caculateVisionLines();

    //Entity* lineToEntityCollision(vec2f lineA, vec2f lineB, vec)
    /**
     * @brief Called in the update method. Calculates when to split the cell.
     */
    void splitCell();
};

#endif // CELL_H_INCLUDE
