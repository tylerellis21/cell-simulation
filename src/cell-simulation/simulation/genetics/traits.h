#ifndef TRAITS_H_INCLUDE
#define TRAITS_H_INCLUDE

// Project includes.
#include "../../typedefs.h"

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
};

#endif // TRAITS_H_INCLUDE
