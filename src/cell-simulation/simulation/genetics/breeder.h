#ifndef BREEDER_H_INCLUDE
#define BREEDER_H_INCLUDE

// Project includes.
#include "dna.h"
#include "genome.h"
#include "traits.h"

/**
 * @brief This class handles the genome breeding.
 */
class Breeder
{
public:

    /**
     * @brief Replicate the parent dna with random mutations.
     * @param parent = The dna genome.
     * @return The dna genome.
     */
    static DNA replicate(const DNA& parent);

private:

    static Genome replicateGenome(const DNA& parent);
};

#endif // BREEDER_H_INCLUDE
