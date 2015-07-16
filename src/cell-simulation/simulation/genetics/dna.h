#ifndef DNA_H_INCLUDE
#define DNA_H_INCLUDE

#include "traits.h"
#include "genome.h"

/**
 * @brief Holds all of the genetic data used to describe a cell.
 */
struct DNA
{

    DNA();

    DNA(Genome&& genome, Traits traits);

    /**
     * @brief The genome weights used for the neural network.
     */
    Genome genome;

    /**
     * @brief The actual traits of the cell.
     */
    Traits traits;
};

#endif // DNA_H_INCLUDE
