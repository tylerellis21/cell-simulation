#ifndef BREEDER_H_INCLUDE
#define BREEDER_H_INCLUDE

// Project includes.
#include "genome.h"

/**
 * @brief This class handles the genome breeding.
 */
class Breeder
{
public:

    /**
     * @brief Replicate the parent genome with random mutations.
     * @param parent = The parent genome.
     * @return The replicated genome.
     */
    static Genome replicate(const Genome& parent);
};

#endif // BREEDER_H_INCLUDE
