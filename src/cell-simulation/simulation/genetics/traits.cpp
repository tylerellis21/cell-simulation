#include "traits.h"
#include "../../mathutils.h"
#include "../randomgen.h"

// Standard includes.
#include <stdlib.h>

Traits::Traits() :
    mutationRate(RandomGen::randomInt(0, 10000)),
    splitRate(RandomGen::randomFloat(10.0f, 60.0f)),
    red(RandomGen::randomFloat(0.0f, 1.0f)),
    green(RandomGen::randomFloat(0.0f, 1.0f)),
    blue(RandomGen::randomFloat(0.0f, 1.0f)),
    eyeOffsetA(RandomGen::randomFloat(0.0f, nx::PiOver2)),
    eyeOffsetB(RandomGen::randomFloat(0.0f, nx::PiOver2)),
    eyeLengthA(RandomGen::randomFloat(minEyeLength, maxEyeLength)),
    eyeLengthB(RandomGen::randomFloat(minEyeLength, maxEyeLength)),
    eyeLengthC(RandomGen::randomFloat(minEyeLength, maxEyeLength))
{ }
