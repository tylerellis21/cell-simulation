#ifndef RANDOMGEN_H_INCLUDE
#define RANDOMGEN_H_INCLUDE

#include <nex/math/mathhelper.h>

#include <random>

#include "../typedefs.h"

class RandomGen {
public:

    static real32 randomFloat(const real32 min, const real32 max);

    static int32 randomInt(const int32 min, const int32 max);

private:

    static std::random_device m_device;
    static std::mt19937 m_engine;

};

#endif // RANDOMGEN_H_INCLUDE
