#include "ai/NeuralNet.h"
#include <cmath>

using namespace agarai;
using namespace std;
////////////////////////////////////////////////////////////////////////////////
array<float, Action::NUM_ACTIONS> NeuralNet::feedForward(const Sensor* const sensor)
{
    // This container is used to return values for all the action outputs. This array
    // contains one value per action neuron, which is the sum of all its weighted
    // input connections. The sum has an arbitrary range. Return by value assumes compiler
    // return value optimization.
    std::array<float, Action::NUM_ACTIONS> actionLevels;
    actionLevels.fill(0.0); // undriven actions default to value 0.0

    // Weighted inputs to each neuron are summed in neuronAccumulators[]
    std::vector<float> neuronAccumulators(neurons.size(), 0.0);

    // Connections were ordered at birth so that all connections to neurons get
    // processed here before any connections to actions. As soon as we encounter the
    // first connection to an action, we'll pass all the neuron input accumulators
    // through a transfer function and update the neuron outputs in the indiv,
    // except for undriven neurons which act as bias feeds and don't change. The
    // transfer function will leave each neuron's output in the range -1.0..1.0.

    bool neuronOutputsComputed = false;
    for (Gene & conn : connections) {
        if (conn.sinkType == ACTION && !neuronOutputsComputed) {
            // We've handled all the connections from sensors and now we are about to
            // start on the connections to the action outputs, so now it's time to
            // update and latch all the neuron outputs to their proper range (-1.0..1.0)
            for (unsigned neuronIndex = 0; neuronIndex < neurons.size(); ++neuronIndex) {
                if (neurons[neuronIndex].driven) {
                    neurons[neuronIndex].output = std::tanh(neuronAccumulators[neuronIndex]);
                }
            }
            neuronOutputsComputed = true;
        }

        // Obtain the connection's input value from a sensor neuron or other neuron
        // The values are summed for now, later passed through a transfer function
        float inputVal;
        if (conn.sourceType == SENSOR) {
            inputVal = sensor->get((SensorType)conn.sourceNum);
        } else {
            inputVal = neurons[conn.sourceNum].output;
        }

        // Weight the connection's value and add to neuron accumulator or action accumulator.
        // The action and neuron accumulators will therefore contain +- float values in
        // an arbitrary range.
        if (conn.sinkType == ACTION) {
            actionLevels[conn.sinkNum] += inputVal * conn.weightAsFloat();
        } else {
            neuronAccumulators[conn.sinkNum] += inputVal * conn.weightAsFloat();
        }
    }

    return actionLevels;
}