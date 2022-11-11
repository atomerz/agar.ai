#include "ai/Genome.h"
#include "ai/Params.h"

using namespace agarai;

Genome agarai::makeRandomGenome() {
  Genome genome;

  unsigned length = randomUint(GENOME_INITIAL_LENGTH_MIN, GENOME_INITIAL_LENGTH_MAX);
  for (unsigned n = 0; n < length; ++n) {
    genome.push_back(makeRandomGene());
  }

  return genome;
}