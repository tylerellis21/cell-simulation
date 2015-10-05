#ifndef TRAITS_H_INCLUDE
#define TRAITS_H_INCLUDE

// Project includes.
#include "../../typedefs.h"

const real32 minEyeLength = 48.0f;
const real32 maxEyeLength = 128.0f;

struct Traits
{
    Traits();

    int32 mutationRate;

    real32 splitRate;

    real32 red;
    real32 green;
    real32 blue;

    real32 eyeOffsetA;
    real32 eyeOffsetB;

    real32 eyeLengthA;
    real32 eyeLengthB;
    real32 eyeLengthC;
};

#endif // TRAITS_H_INCLUDE
