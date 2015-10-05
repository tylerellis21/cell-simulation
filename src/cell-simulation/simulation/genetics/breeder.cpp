#include "breeder.h"
#include "../../mathutils.h"
#include "../../core/console.h"
#include "../randomgen.h"
#include <sstream>
#include <stdlib.h>

DNA Breeder::replicate(const DNA& parent)
{
    Genome replicatedGenome = replicateGenome(parent);
    Traits newTraits;

    newTraits.mutationRate = parent.traits.mutationRate + RandomGen::randomInt(-5, 5);

    newTraits.splitRate =
            nx::clamp(parent.traits.splitRate + RandomGen::randomFloat(-2.0f, 2.0f),
                      30.0f, 100.0f);

    const real32 colorChange = 0.01f;

    newTraits.red = nx::clamp(parent.traits.red + RandomGen::randomFloat(-colorChange, colorChange), 0.f, 1.f);
    newTraits.green = nx::clamp(parent.traits.green + RandomGen::randomFloat(-colorChange, colorChange), 0.f, 1.f);
    newTraits.blue = nx::clamp(parent.traits.blue + RandomGen::randomFloat(-colorChange, colorChange), 0.f, 1.f);

    const real32 eyeLengthChange = 0.001f;

    newTraits.eyeLengthA = nx::clamp(parent.traits.eyeLengthA + RandomGen::randomFloat(-eyeLengthChange, eyeLengthChange),
                                     minEyeLength, maxEyeLength);

    newTraits.eyeLengthB = nx::clamp(parent.traits.eyeLengthB + RandomGen::randomFloat(-eyeLengthChange, eyeLengthChange),
                                     minEyeLength, maxEyeLength);

    newTraits.eyeLengthC = nx::clamp(parent.traits.eyeLengthC + RandomGen::randomFloat(-eyeLengthChange, eyeLengthChange),
                                     minEyeLength, maxEyeLength);

    const real32 eyeOffsetChange = 0.001f;

    newTraits.eyeOffsetA = nx::clamp(parent.traits.eyeOffsetA + RandomGen::randomFloat(-eyeOffsetChange, eyeOffsetChange), 0.f, nx::PiOver4);
    newTraits.eyeOffsetB = nx::clamp(parent.traits.eyeOffsetB + RandomGen::randomFloat(-eyeOffsetChange, eyeOffsetChange), 0.f, nx::PiOver4);

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

        const int32 dice = RandomGen::randomInt(0, parent.traits.mutationRate);
        if (dice >= 670) {
            weights[i] = parentWeights[i];
        }
        else if (dice >= 335) {
            weights[i] = parentWeights[i] + RandomGen::randomFloat(-0.5f, 0.5f);
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
