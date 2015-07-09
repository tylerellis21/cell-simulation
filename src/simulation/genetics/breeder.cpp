#include "breeder.h"
#include "../../util/mathutils.h"
#include "../../util/log.h"

#include <stdlib.h>

Genome Breeder::replicate(const Genome& parent)
{
    Genome genome;

    const uint32 size = parent.getLength();

    const real32* parentWeights = parent.readWeights();

    int mutationCount = 0;

    real32* weights = genome.editWeights();
    for (int32 i = 0; i < size; i++) {

        int dice = rand() % 100;

        // Most of the time we directly copy.
        if (dice >= 25) {
            weights[i] = parentWeights[i];
        }
        // Copy with a mutation.
        else {
            weights[i] = parentWeights[i] + randomFloat();
            mutationCount += 1;
        }
    }

    std::stringstream sb;
    sb << "Mutation count: " << mutationCount;

    Log::info(sb.str());

    return genome;
}
