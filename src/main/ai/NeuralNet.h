#pragma once
#include <vector>
#include <array>
#include "ai/Gene.h"
#include "ai/Action.h"
#include "ai/Sensor.h"
////////////////////////////////////////////////////////////////////////////////
namespace agarai
{
  ////////////////////////////////////////////////////////////////////////////////
  class NeuralNet {
    private:
    std::vector<Gene> connections; // connections are equivalent to genes

    struct Neuron {
        float output;
        bool driven;        // undriven neurons have fixed output values
    };
    std::vector<Neuron> neurons;

    public:
    std::array<float, Action::NUM_ACTIONS>  feedForward(Sensor* sensor);
  };
}