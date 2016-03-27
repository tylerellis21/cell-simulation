#ifndef GENOME_H_INCLUDE
#define GENOME_H_INCLUDE

#include <scl/types.h>

// Project includes.
#include "../../mathutils.h"
#include "../randomgen.h"
/**
 * @brief This class stores the weight data used for the nerual network.
 */
class Genome
{
public:
    friend class GenomePool;

    /**
     * @brief The default genome constructor. (Generates a random set of weights)
     */
    Genome();

    /**
     * @brief The genome copy constructor.
     * @param other = The genome to copy from.
     */
    Genome(const Genome& other);

    /**
     * @brief The gneome move constructor.
     * @param other = The genome to move from.
     */
    Genome(Genome&& other);

    /**
     * @brief The default genome destructor.
     */
    ~Genome();

    /**
     * @brief The genome copy assignment operator.
     * @param other = The genome to assign to this one.
     * @return This genome.
     */
    Genome& operator=(const Genome& other);

    /**
     * @brief The genome move assignment operator.
     * @param other = The other genome to move into this one.
     * @return The new genome.
     */
    Genome& operator=(Genome&& other);

    /**
     * @brief Get a pointer to the weights which can be written to here.
     * @return A pointer to the data.
     */
    r32* editWeights() { return m_weights; }

    /**
     * @brief Get a read only pointer to the weight data.
     * @return A const pointer to the data.
     */
    const r32* readWeights() const { return m_weights; }

    /**
     * @brief Get the length of the genome float array.
     * @return The length of the array.
     */
    i32 getLength() const { return m_length; }

    /**
     * @brief Generate a random inital genome weight.
     * @return The random weight value.
     */
    inline static r32 randomGenomeWeight() { return RandomGen::randomFloat(-1.0f, 1.0f); }

private:

    /**
     * @brief The length of the weight array.
     */
    u32 m_length;

    /**
     * @brief The actual genome data buffer.
     */
    r32* m_weights;
};

#endif // GENOME_H_INCLUDE
