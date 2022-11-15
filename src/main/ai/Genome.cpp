#include "ai/Genome.h"
#include "ai/Params.h"

#include <iostream>
#include <iomanip>
#include <cstring>

using namespace agarai;

Genome::Genome(): generation(0) {
}

Genome::~Genome() {
}

std::unique_ptr<Genome> Genome::random() {
  auto genome = std::unique_ptr<Genome>(new Genome());

  unsigned length = randomUint(GENOME_INITIAL_LENGTH_MIN, GENOME_INITIAL_LENGTH_MAX);
  for (unsigned n = 0; n < length; ++n) {
    genome->genes.push_back(makeRandomGene());
  }

  return std::move(genome);
}

// If the genome is longer than the prescribed length, and if it's longer
// than one gene, then we remove genes from the front or back. This is
// used only when the simulator is configured to allow genomes of
// unequal lengths during a simulation.
void Genome::cropLength(unsigned length) {
  if (genes.size() > length && length > 0) {
    if (randomUint() / (float)RAND_MAX < 0.5) {
      // trim front
      unsigned numberElementsToTrim = genes.size() - length;
      genes.erase(genes.begin(), genes.begin() + numberElementsToTrim);
    } else {
      // trim back
      genes.erase(genes.end() - (genes.size() - length), genes.end());
    }
  }
}

// This applies a point mutation at a random bit in a genome.
void Genome::randomBitFlip() {
  int method = 1;

  unsigned byteIndex = randomUint(0, genes.size() - 1) * sizeof(Gene);
  unsigned elementIndex = randomUint(0, genes.size() - 1);
  uint8_t bitIndex8 = 1 << randomUint(0, 7);

  if (method == 0) {
    ((uint8_t *)&genes[0])[byteIndex] ^= bitIndex8;
  } else if (method == 1) {
    float chance = randomUint() / (float)RAND_MAX; // 0..1
    if (chance < 0.2) { // sourceType
        genes[elementIndex].sourceType ^= 1;
    } else if (chance < 0.4) { // sinkType
        genes[elementIndex].sinkType ^= 1;
    } else if (chance < 0.6) { // sourceNum
        genes[elementIndex].sourceNum ^= bitIndex8;
    } else if (chance < 0.8) { // sinkNum
        genes[elementIndex].sinkNum ^= bitIndex8;
    } else { // weight
        genes[elementIndex].weight ^= (1 << randomUint(1, 15));
    }
  } else {
    assert(false);
  }
}

// Inserts or removes a single gene from the genome. This is
// used only when the simulator is configured to allow genomes of
// unequal lengths during a simulation.
void Genome::randomInsertDeletion() {
  float probability = GENE_INSERTION_DELETION_RATE;
  if (randomUint() / (float)RAND_MAX < probability) {
    if (randomUint() / (float)RAND_MAX < DELETION_RATIO) {
      // deletion
      if (genes.size() > 1) {
          genes.erase(genes.begin() + randomUint(0, genes.size() - 1));
      }
    } else if (genes.size() < GENOME_MAX_LENGTH) {
      // insertion
      //genes.insert(genes.begin() + randomUint(0, genes.size() - 1), makeRandomGene());
      genes.push_back(makeRandomGene());
    }
  }
}

// This function causes point mutations in a genome with a probability defined
// by the parameter p.pointMutationRate.
void Genome::applyPointMutations() {
  unsigned numberOfGenes = genes.size();
  while (numberOfGenes-- > 0) {
    if ((randomUint() / (float)RAND_MAX) < POINT_MUTATION_RATE) {
      randomBitFlip();
    }
  }
}

std::unique_ptr<Genome> Genome::childGenome(const std::vector<Genome>& parentGenomes) {
  // random parent (or parents if sexual reproduction) with random
  // mutations
  auto genome = std::unique_ptr<Genome>(new Genome());

  uint16_t parent1Idx;
  uint16_t parent2Idx;

  // Choose two parents randomly from the candidates. If the parameter
  // p.chooseParentsByFitness is false, then we choose at random from
  // all the candidate parents with equal preference. If the parameter is
  // true, then we give preference to candidate parents according to their
  // score. Their score was computed by the survival/selection algorithm
  // in survival-criteria.cpp.
  if (CHOOSE_PARENT_BY_FITNESS && (parentGenomes.size() / 3) > 1) {
    parent1Idx = randomUint(1, (parentGenomes.size() / 3) - 1);
    parent2Idx = randomUint(0, parent1Idx - 1);
  } else {
    parent1Idx = randomUint(0, parentGenomes.size() - 1);
    parent2Idx = randomUint(0, parentGenomes.size() - 1);
  }

  auto& p1 = parentGenomes[parent1Idx];
  auto& p2 = parentGenomes[parent2Idx];
  auto &g1 = p1.genes;
  auto &g2 = p2.genes;

  if (g1.empty() || g2.empty()) {
    assert(false);
  }

  genome->generation = ((p1.generation > p2.generation) ? p1.generation : p2.generation) + 1;

  auto overlayWithSliceOf = [&](auto& gShorter) {
    uint16_t index0 = randomUint(0, gShorter.size() - 1);
    uint16_t index1 = randomUint(0, gShorter.size());
    if (index0 > index1) {
      std::swap(index0, index1);
    }
    std::copy(gShorter.begin() + index0, gShorter.begin() + index1, genome->genes.begin() + index0);
  };

  if (SEXUAL_REPRODUCTION) {
    if (g1.size() > g2.size()) {
      genome->genes = g1;
      overlayWithSliceOf(g2);
      assert(!genome->genes.empty());
    } else {
      genome->genes = g2;
      overlayWithSliceOf(g1);
      assert(!genome->genes.empty());
    }

    // Trim to length = average length of parents
    unsigned sum = g1.size() + g2.size();
    // If average length is not an integral number, add one half the time
    if ((sum & 1) && (randomUint() & 1)) {
      ++sum;
    }
    genome->cropLength(sum / 2);
    assert(!genome->genes.empty());
  } else {
    genome->genes = g2;
    assert(!genome->genes.empty());
  }

    genome->randomInsertDeletion();
    assert(!genome->genes.empty());
    genome->applyPointMutations();
    assert(!genome->genes.empty());
    assert(genome->genes.size() <= GENOME_MAX_LENGTH);

    return std::move(genome);
}

const std::vector<Gene>& Genome::getGenes() const {
  return genes;
}

const uint16_t Genome::getGeneration() const {
  return generation;
}

// Format: 32-bit hex strings, one per gene
void Genome::print() const {
  constexpr unsigned genesPerLine = 8;
  unsigned count = 0;
  for (Gene gene : genes) {
    if (count == genesPerLine) {
      std::cout << std::endl;
      count = 0;
    } else if (count != 0) {
      std::cout << " ";
    }

    assert(sizeof(Gene) == 4);
    uint32_t n;
    std::memcpy(&n, &gene, sizeof(n));
    std::cout << std::hex << std::setfill('0') << std::setw(8) << n;
    ++count;
  }
  std::cout << std::dec << std::endl;
}
