#ifndef RANDOMGEN_H_INCLUDE
#define RANDOMGEN_H_INCLUDE

#include <scl/types.h>
#include <scl/math/help.h>

#include <random>

class RandomGen {
public:

    static r32 randomFloat(const r32 min, const r32 max);

    static i32 randomInt(const i32 min, const i32 max);

private:

    static std::random_device m_device;
    static std::mt19937 m_engine;

};

#endif // RANDOMGEN_H_INCLUDE
