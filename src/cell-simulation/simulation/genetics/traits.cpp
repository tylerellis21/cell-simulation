#include "traits.h"
#include "../../mathutils.h"

// Standard includes.
#include <stdlib.h>

Traits::Traits() :
    mutationRate(rand() % 10005),
    splitRate(randomFloat(10.0f, 60.0f)),
    red(randomFloat(0.0f)),
    green(randomFloat(0.0f)),
    blue(randomFloat(0.0f))
{ }
