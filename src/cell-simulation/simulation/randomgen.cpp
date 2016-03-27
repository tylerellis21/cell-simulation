#include "randomgen.h"

std::random_device RandomGen::m_device;
std::mt19937 RandomGen::m_engine(m_device());

r32 RandomGen::randomFloat(const r32 min, const r32 max) {
    std::uniform_real_distribution<r32> dist(min, max);
    return dist(m_engine);
}

i32 RandomGen::randomInt(const i32 min, const i32 max) {
    std::uniform_int_distribution<i32> dist(min, max);
    return dist(m_engine);
}
