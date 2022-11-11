#pragma once

#include <vector>
#include <map>
#include <list>

#include "ai/Gene.h"

namespace agarai {

  // An individual's genome is a set of Genes (see Gene comments above). Each
  // gene is equivalent to one connection in a neural net. An individual's
  // neural net is derived from its set of genes.
  typedef std::vector<Gene> Genome;

  // This structure is used while converting the connection list to a
  // neural net. This helps us to find neurons that don't feed anything
  // so that they can be removed along with all the connections that
  // feed the useless neurons. We'll cull neurons with .numOutputs == 0
  // or those that only feed themselves, i.e., .numSelfInputs == .numOutputs.
  // Finally, we'll renumber the remaining neurons sequentially starting
  // at zero using the .remappedNumber member.
  struct Node {
    uint16_t remappedNumber;
    uint16_t numOutputs;
    uint16_t numSelfInputs;
    uint16_t numInputsFromSensorsOrOtherNeurons;
  };

  // Two neuron renumberings occur: The original genome uses a uint16_t for
  // neuron numbers. The first renumbering maps 16-bit unsigned neuron numbers
  // to the range 0..MAX_NUMBER_NEURONS - 1. After culling useless neurons
  // (see comments above), we'll renumber the remaining neurons sequentially
  // starting at 0.
  typedef std::map<uint16_t, Node> NodeMap; // key is neuron number 0..MAX_NUMBER_NEURONS - 1

  typedef std::list<Gene> ConnectionList;


  // Returns by value a single genome with random genes.
  Genome makeRandomGenome();

}