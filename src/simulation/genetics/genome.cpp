#include "genome.h"

// Standard includes.
#include <algorithm>

// Project includes.
#include "../world.h"

// Nex includes.
#include <nex/math/mathhelper.h>

// Here we are adding two extra values to the genome data.
// The first two values are the split rate, and the genome mutation rate.
// The rest of the data corrisponds to the input and output weights and biases.

// Default constructor.
Genome::Genome() :
    m_length(World::m_weightCount + 5),
    m_weights(new real32[World::m_weightCount + 5])
{
    // Move theses first values into a trait class?

    // Set the inital split rate to 10 seconds.
    m_weights[CELL_SPLIT_RATE_INDEX] = (((10.0f + randomFloat(-1.0f, 500.0f)) / 500.0f) * 2.0f) - 1.0f;

    // This is the mutation rate.
    m_weights[CELL_MUTATION_RATE_INDEX] = (((100.0f + randomFloat(-50.0f, 100000.0f)) / 100000.0f) * 2.0f) - 1.0f;

    m_weights[CELL_RED_COLOR_INDEX] = randomFloat(0.0f);
    m_weights[CELL_GREEN_COLOR_INDEX] = randomFloat(0.0f);
    m_weights[CELL_BLUE_COLOR_INDEX] = randomFloat(0.0f);


    for (uint32 i = 5; i < m_length; i++) {
        m_weights[i] = Genome::randomGenomeWeight();
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

int32 Genome::unscaleMutationRate(real32 mutationRate)
{
    mutationRate = (nx::clamp(mutationRate, -1.0f, 1.0f) + 1.0f) * 0.5f;

    return int32(mutationRate * 100000.0f);
}

int32 Genome::unscaleSplitRate(real32 splitRate)
{
    splitRate = (nx::clamp(splitRate, -1.0f, 1.0f) + 1.0f) * 0.5f;

    return int32(splitRate * 1000.0f);
}
