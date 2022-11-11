#include "ai/Gene.h"

using namespace agarai;

// Returns by value a single gene with random members.
// See genome.h for the width of the members.
// ToDo: don't assume the width of the members in gene.
Gene agarai::makeRandomGene() {
    Gene gene;

    gene.sourceType = randomUint() & 1;
    gene.sourceNum = (uint16_t)randomUint(0, 0x7fff);
    gene.sinkType = randomUint() & 1;
    gene.sinkNum = (uint16_t)randomUint(0, 0x7fff);
    gene.weight = Gene::makeRandomWeight();

    return gene;
}