#include "breeder.h"
#include "../../util/mathutils.h"
#include "../../core/console.h"

#include <sstream>
#include <stdlib.h>

Genome Breeder::replicate(const Genome& parent)
{
    Genome genome;

    const uint32 size = parent.getLength();

    const real32* parentWeights = parent.readWeights();

    // Rescale the mutation rate back up since it is stored as a float value.
    int32 mutationRate = Genome::unscaleMutationRate(parentWeights[CELL_MUTATION_RATE_INDEX]);

    // Make sure we don't go under the minimum value.
    if (mutationRate < 1) {
        mutationRate = 1;
    }

    int32 mutationCount = 0;

    // Copy from parent, fine tune from parent, and randomized
    // You can thank waterlimon for the advice we got here.

    // TODO: Improve this method??

    real32* weights = genome.editWeights();
    for (int32 i = 0; i < size; i++) {


        int dice = rand() % mutationRate;
        if (dice >= 50) {

            // Most of the time we directly copy.
            weights[i] = parentWeights[i];
        }
        else if (dice <= 25) {

            // Copy with a mutation.
            weights[i] = parentWeights[i] + randomFloat(-0.02f, 0.02f);
            mutationCount += 1;
        }
        else {
            // Generate a new random genome weight.
            weights[i] = Genome::randomGenomeWeight();
            mutationCount += 1;
        }
    }

    std::stringstream sb;
    sb << "mutation count: " << mutationCount;
    Console::write(sb.str());

    return genome;
}
