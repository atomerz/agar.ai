#pragma once

#include <vector>

#include "DecisionContext.h"
#include "ai/NeuralNet.h"
#include "ai/Sensor.h"

namespace agarai {

class Bubble;

class Brain {
  public:
    Brain(Bubble* target, Genome genome);
    ~Brain();

    void decide(const DecisionContext& context);

    Genome getGenome() const;
    NeuralNet getNeuralNet() const;

  private:
    Bubble* target;
    
    Genome genome;
    NeuralNet neuralNet;
};

}