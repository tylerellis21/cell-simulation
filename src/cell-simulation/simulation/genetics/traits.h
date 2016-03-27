#ifndef TRAITS_H_INCLUDE
#define TRAITS_H_INCLUDE

#include <scl/types.h>

const r32 minEyeLength = 48.0f;
const r32 maxEyeLength = 128.0f;

struct Traits
{
    Traits();

    i32 mutationRate;

    r32 splitRate;

    r32 red;
    r32 green;
    r32 blue;

    r32 eyeOffsetA;
    r32 eyeOffsetB;

    r32 eyeLengthA;
    r32 eyeLengthB;
    r32 eyeLengthC;
};

#endif // TRAITS_H_INCLUDE
