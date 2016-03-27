#include "genome.h"

// Standard includes.
#include <algorithm>

// Project includes.
#include "../world.h"

// SCL includes.
#include <scl/math/help.h>

// Here we are adding two extra values to the genome data.
// The first two values are the split rate, and the genome mutation rate.
// The rest of the data corrisponds to the input and output weights and biases.

// Default constructor.
Genome::Genome() :
    m_length(World::m_weightCount),
    m_weights(new r32[World::m_weightCount]) {
    // Move theses first values into a trait class?
    for (u32 i = 0; i < m_length; i++) {
        m_weights[i] = Genome::randomGenomeWeight();
    }
}

// Copy constructor.
Genome::Genome(const Genome& other) :
    m_length(other.m_length),
    m_weights(new r32[other.m_length]) {
    std::copy(other.m_weights, other.m_weights + m_length, m_weights);
}

// Move constructor.
Genome::Genome(Genome&& other) :
    m_length(0),
    m_weights(0) {
    *this = std::move(other);
}

// Default destructor.
Genome::~Genome() {
    if (m_weights) {
        delete [] m_weights;
        m_weights = 0;
    }
}

// Copy assignment operator.
Genome& Genome::operator=(const Genome& other) {
    if (this != &other) {

        if (m_weights) {
            delete m_weights;
            m_weights = 0;
        }

        m_length = other.m_length;
        m_weights = new r32[m_length];

        std::copy(other.m_weights, other.m_weights + m_length, m_weights);
    }

    return *this;
}

// Move assignment operator.
Genome& Genome::operator =(Genome&& other) {
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
