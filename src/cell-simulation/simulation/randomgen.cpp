#include "randomgen.h"

std::random_device RandomGen::m_device;
std::mt19937 RandomGen::m_engine(m_device());

/*static std::uniform_real_distribution<real32> m_realDist;
    static std::uniform_real_distribution<real32> m_piDist;
    static std::uniform_int_distribution<int32> m_intDist;*/


real32 RandomGen::randomFloat(const real32 min, const real32 max)
{
    std::uniform_real_distribution<real32> dist(min, max);
    return dist(m_engine);
}

int32 RandomGen::randomInt(const int32 min, const int32 max)
{
    std::uniform_int_distribution<int32> dist(min, max);
    return dist(m_engine);
}
