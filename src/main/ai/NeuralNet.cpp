#include "ai/NeuralNet.h"
#include "ai/Genome.h"
#include "ai/Params.h"
#include "ai/Sensor.h"

#include <cassert>
#include <cmath>
#include <iostream>

using namespace agarai;
using namespace std;

NeuralNet::NeuralNet(Genome _genome) {
  createWiringFromGenome(_genome);
}

NeuralNet::~NeuralNet() {
}

array<float, Action::NUM_ACTIONS> NeuralNet::feedForward(Sensor* sensor) {
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

void NeuralNet::printIGraphEdgeList() const {
    for (auto & conn : connections) {
        if (conn.sourceType == SENSOR) {
            std::cout << sensorShortName((SensorType)(conn.sourceNum));
        } else {
            std::cout << "N" << std::to_string(conn.sourceNum);
        }

        std::cout << " ";

        if (conn.sinkType == ACTION) {
            std::cout << actionShortName((Action)(conn.sinkNum));
        } else {
            std::cout << "N" << std::to_string(conn.sinkNum);
        }

        std::cout << " " << std::to_string(conn.weight) << std::endl;
    }
}

// Convert the indiv's genome to a renumbered connection list.
// This renumbers the neurons from their uint16_t values in the genome
// to the range 0..MAX_NUMBER_NEURONS - 1 by using a modulo operator.
// Sensors are renumbered 0..Sensor::NUM_SENSES - 1
// Actions are renumbered 0..Action::NUM_ACTIONS - 1
static void makeRenumberedConnectionList(ConnectionList &connectionList, const Genome &genome) {
  connectionList.clear();
  for (auto const &gene : genome) {
    connectionList.push_back(gene);
    auto &conn = connectionList.back();

    if (conn.sourceType == NEURON) {
      conn.sourceNum %= MAX_NUMBER_NEURONS;
    } else {
      conn.sourceNum %= SensorType::NUM_SENSES;
    }

    if (conn.sinkType == NEURON) {
      conn.sinkNum %= MAX_NUMBER_NEURONS;
    } else {
      conn.sinkNum %= Action::NUM_ACTIONS;
    }
  }
}

// Scan the connections and make a list of all the neuron numbers
// mentioned in the connections. Also keep track of how many inputs and
// outputs each neuron has.
static void makeNodeList(NodeMap &nodeMap, const ConnectionList &connectionList) {
    nodeMap.clear();

    for (const Gene &conn : connectionList) {
        if (conn.sinkType == NEURON) {
            auto it = nodeMap.find(conn.sinkNum);
            if (it == nodeMap.end()) {
                assert(conn.sinkNum < MAX_NUMBER_NEURONS);
                nodeMap.insert(std::pair<uint16_t, Node>(conn.sinkNum, {} ));
                it = nodeMap.find(conn.sinkNum);
                assert(it->first < MAX_NUMBER_NEURONS);
                it->second.numOutputs = 0;
                it->second.numSelfInputs = 0;
                it->second.numInputsFromSensorsOrOtherNeurons = 0;
            }

            if (conn.sourceType == NEURON && (conn.sourceNum == conn.sinkNum)) {
                ++(it->second.numSelfInputs);
            } else {
                ++(it->second.numInputsFromSensorsOrOtherNeurons);
            }
            assert(nodeMap.count(conn.sinkNum) == 1);
        }
        if (conn.sourceType == NEURON) {
            auto it = nodeMap.find(conn.sourceNum);
            if (it == nodeMap.end()) {
                assert(conn.sourceNum < MAX_NUMBER_NEURONS);
                nodeMap.insert(std::pair<uint16_t, Node>(conn.sourceNum, {} ));
                it = nodeMap.find(conn.sourceNum);
                assert(it->first < MAX_NUMBER_NEURONS);
                it->second.numOutputs = 0;
                it->second.numSelfInputs = 0;
                it->second.numInputsFromSensorsOrOtherNeurons = 0;
            }
            ++(it->second.numOutputs);
            assert(nodeMap.count(conn.sourceNum) == 1);
        }
    }
}

// During the culling process, we will remove any neuron that has no outputs,
// and all the connections that feed the useless neuron.
static void removeConnectionsToNeuron(ConnectionList &connections, NodeMap &nodeMap, uint16_t neuronNumber)
{
    for (auto itConn = connections.begin(); itConn != connections.end(); ) {
        if (itConn->sinkType == NEURON && itConn->sinkNum == neuronNumber) {
            // Remove the connection. If the connection source is from another
            // neuron, also decrement the other neuron's numOutputs:
            if (itConn->sourceType == NEURON) {
                --(nodeMap[itConn->sourceNum].numOutputs);
            }
            itConn = connections.erase(itConn);
        } else {
            ++itConn;
        }
    }
}

// If a neuron has no outputs or only outputs that feed itself, then we
// remove it along with all connections that feed it. Reiterative, because
// after we remove a connection to a useless neuron, it may result in a
// different neuron having no outputs.
static void cullUselessNeurons(ConnectionList &connections, NodeMap &nodeMap)
{
    bool allDone = false;
    while (!allDone) {
        allDone = true;
        for (auto itNeuron = nodeMap.begin(); itNeuron != nodeMap.end(); ) {
            assert(itNeuron->first < MAX_NUMBER_NEURONS);
            // We're looking for neurons with zero outputs, or neurons that feed itself
            // and nobody else:
            if (itNeuron->second.numOutputs == itNeuron->second.numSelfInputs) {  // could be 0
                allDone = false;
                // Find and remove connections from sensors or other neurons
                removeConnectionsToNeuron(connections, nodeMap, itNeuron->first);
                itNeuron = nodeMap.erase(itNeuron);
            } else {
                ++itNeuron;
            }
        }
    }
}

// When a new population is generated and every individual is given a
// neural net, the neuron outputs must be initialized to something:
//constexpr float initialNeuronOutput() { return (NEURON_RANGE / 2.0) + NEURON_MIN; }
static constexpr float initialNeuronOutput() { return 0.5; }

// This function is used when an agent is spawned. This function converts the
// agent's inherited genome into the agent's neural net brain. There is a close
// correspondence between the genome and the neural net, but a connection
// specified in the genome will not be represented in the neural net if the
// connection feeds a neuron that does not itself feed anything else.
// Neurons get renumbered in the process:
// 1. Create a set of referenced neuron numbers where each index is in the
//    range 0..p.genomeMaxLength-1, keeping a count of outputs for each neuron.
// 2. Delete any referenced neuron index that has no outputs or only feeds itself.
// 3. Renumber the remaining neurons sequentially starting at 0.
void NeuralNet::createWiringFromGenome(Genome genome) {
    NodeMap nodeMap;  // list of neurons and their number of inputs and outputs
    ConnectionList connectionList; // synaptic connections

    // Convert the indiv's genome to a renumbered connection list
    makeRenumberedConnectionList(connectionList, genome);

    // Make a node (neuron) list from the renumbered connection list
    makeNodeList(nodeMap, connectionList);

    // Find and remove neurons that don't feed anything or only feed themself.
    // This reiteratively removes all connections to the useless neurons.
    cullUselessNeurons(connectionList, nodeMap);

    // The neurons map now has all the referenced neurons, their neuron numbers, and
    // the number of outputs for each neuron. Now we'll renumber the neurons
    // starting at zero.

    assert(nodeMap.size() <= MAX_NUMBER_NEURONS);
    uint16_t newNumber = 0;
    for (auto & node : nodeMap) {
        assert(node.second.numOutputs != 0);
        node.second.remappedNumber = newNumber++;
    }

    // Create the indiv's connection list in two passes:
    // First the connections to neurons, then the connections to actions.
    // This ordering optimizes the feed-forward function in feedForward.cpp.

    connections.clear();

    // First, the connections from sensor or neuron to a neuron
    for (auto const &conn : connectionList) {
        if (conn.sinkType == NEURON) {
            connections.push_back(conn);
            auto &newConn = connections.back();
            // fix the destination neuron number
            newConn.sinkNum = nodeMap[newConn.sinkNum].remappedNumber;
            // if the source is a neuron, fix its number too
            if (newConn.sourceType == NEURON) {
                newConn.sourceNum = nodeMap[newConn.sourceNum].remappedNumber;
            }
        }
    }

    // Last, the connections from sensor or neuron to an action
    for (auto const &conn : connectionList) {
        if (conn.sinkType == ACTION) {
            connections.push_back(conn);
            auto &newConn = connections.back();
            // if the source is a neuron, fix its number
            if (newConn.sourceType == NEURON) {
                newConn.sourceNum = nodeMap[newConn.sourceNum].remappedNumber;
            }
        }
    }

    // Create the indiv's neural node list
    neurons.clear();
    for (unsigned neuronNum = 0; neuronNum < nodeMap.size(); ++neuronNum) {
        neurons.push_back( {} );
        neurons.back().output = initialNeuronOutput();
        neurons.back().driven = (nodeMap[neuronNum].numInputsFromSensorsOrOtherNeurons != 0);
    }
}