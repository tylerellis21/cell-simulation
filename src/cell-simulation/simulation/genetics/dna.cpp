#include "dna.h"

// Standard includes.
#include <memory>

DNA::DNA() :
    genome(std::move(Genome())),
    traits(Traits())
{ }

DNA::DNA(Genome&& genome, Traits traits) :
    genome(genome),
    traits(traits)
{ }
