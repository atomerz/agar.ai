#pragma once
#include <cstdint>
#include "utils.h"
////////////////////////////////////////////////////////////////////////////////
namespace agarai
{
  constexpr uint8_t SENSOR = 1;  // always a source
  constexpr uint8_t ACTION = 1;  // always a sink
  constexpr uint8_t NEURON = 0;  // can be either a source or sink

  struct Gene { //__attribute__((packed)) Gene {
    uint16_t sourceType:1; // SENSOR or NEURON
    uint16_t sourceNum:7;
    uint16_t sinkType:1;   // NEURON or ACTION
    uint16_t sinkNum:7;
    int16_t weight;

    static constexpr float f1 = 8.0;
    static constexpr float f2 = 64.0;
    //float weightAsFloat() { return std::pow(weight / f1, 3.0) / f2; }
    float weightAsFloat() const { return weight / 8192.0; }
    static int16_t makeRandomWeight() { return randomUint(0, 0xffff) - 0x8000; }
  };

  // An individual's genome is a set of Genes (see Gene comments above). Each
  // gene is equivalent to one connection in a neural net. An individual's
  // neural net is derived from its set of genes.
  typedef std::vector<Gene> Genome;

}