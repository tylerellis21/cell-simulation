#include "genome.h"

// Standard includes.
#include <algorithm>

// Project includes.
#include "../world.h"
#include "../../util/mathutils.h"

// Default constructor.
Genome::Genome() :
    m_length(World::m_weightCount),
    m_weights(new real32[World::m_weightCount])
{
    for (uint32 i = 0; i < m_length; i++) {

        // TODO (Tyler): Test this value range.
        m_weights[i] = randomFloat(-500.0f, 500.0f);
    }
}

// Copy constructor.
Genome::Genome(const Genome& other) :
    m_length(other.m_length),
    m_weights(new real32[other.m_length])
{
    std::copy(other.m_weights, other.m_weights + m_length, m_weights);
}

// Move constructor.
Genome::Genome(Genome&& other) :
    m_length(0),
    m_weights(0)
{
    *this = std::move(other);
}

// Default destructor.
Genome::~Genome()
{
    if (m_weights) {
        delete [] m_weights;
        m_weights = 0;
    }
}

// Copy assignment operator.
Genome& Genome::operator=(const Genome& other)
{
    if (this != &other) {

        if (m_weights) {
            delete m_weights;
            m_weights = 0;
        }

        m_length = other.m_length;
        m_weights = new real32[m_length];

        std::copy(other.m_weights, other.m_weights + m_length, m_weights);
    }

    return *this;
}

// Move assignment operator.
Genome& Genome::operator =(Genome&& other)
{
    if (this != &other) {

        if (m_weights) {
            delete m_weights;
            m_weights = 0;
        }

        m_weights = other.m_weights;
        m_length = other.m_length;

        other.m_weights = 0;
        other.m_length = 0;
    }

    return *this;
}
