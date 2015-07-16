#include "breeder.h"
#include "../../mathutils.h"
#include "../../core/console.h"

#include <sstream>
#include <stdlib.h>

DNA Breeder::replicate(const DNA& parent)
{
    Genome replicatedGenome = replicateGenome(parent);
    Traits newTraits;

    newTraits.mutationRate = parent.traits.mutationRate + (5 - (rand() % 10));
    newTraits.splitRate = parent.traits.splitRate + randomFloat(-2.0f, 2.0f);

    const real32 colorChange = 0.01f;

    newTraits.red = nx::clamp(parent.traits.red + randomFloat(-colorChange, colorChange), 0.0f, 1.0f);
    newTraits.green = nx::clamp(parent.traits.green + randomFloat(-colorChange, colorChange), 0.0f, 1.0f);
    newTraits.blue = nx::clamp(parent.traits.blue + randomFloat(-colorChange, colorChange), 0.0f, 1.0f);

    // Copy directly
    // Copy with an offset
    // Generate a new random weight.

    return DNA(std::move(replicatedGenome), newTraits);
}

Genome Breeder::replicateGenome(const DNA& parent)
{
    Genome newGenome;

    const real32* parentWeights = parent.genome.readWeights();

    real32* weights = newGenome.editWeights();

    int32 mutationCount = 0;

    for (uint32 i = 0; i < newGenome.getLength(); i++) {

        const int32 dice = rand() % parent.traits.mutationRate;
        if (dice >= 670) {
            weights[i] = parentWeights[i];
        }
        else if (dice >= 335) {
            weights[i] = parentWeights[i] + randomFloat(-0.5f, 0.5f);
            mutationCount++;
        }
        else {
            weights[i] = Genome::randomGenomeWeight();
            mutationCount++;
        }
    }

    std::stringstream sb;
    sb << "mutation count: " << mutationCount;
    Console::write(sb.str());

    return newGenome;
}
