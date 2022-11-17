#pragma once

#include <vector>
#include <array>

#include "ai/Action.h"
#include "ai/Genome.h"
#include "ai/Sensor.h"

namespace agarai
{

class NeuralNet {
  public:
    NeuralNet(Genome genome);
    ~NeuralNet();

    std::array<float, Action::NUM_ACTIONS>  feedForward(Sensor* sensor);

    void printIGraphEdgeList(std::ostream& os) const;

  private:
    void createWiringFromGenome(Genome genome);

    std::vector<Gene> connections; // connections are equivalent to genes

    struct Neuron {
        float output;
        bool driven;        // undriven neurons have fixed output values
    };
    std::vector<Neuron> neurons;

};

}
