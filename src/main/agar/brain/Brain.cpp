#include "Brain.h"

#include "SensorImpl.h"

#include "agar/RenderableBubble.h"

using namespace agarai;

Brain::Brain(Bubble* target, Genome _genome): genome(_genome), neuralNet(_genome) {
  this->target = target;
}

Brain::~Brain() {
}

void Brain::decide(const DecisionContext& context) {
  auto sensor = SensorImpl(context);
  auto actions = neuralNet.feedForward(&sensor);

  auto level = actions[Action::SET_DIRECTION_NORTH];
  level = (std::tanh(level) + 1.0) / 2.0; // convert to 0.0..1.0
  if (level > 0.5) {
    target->setDirection(M_PI / 2);
    return;
  }

  level = actions[Action::SET_DIRECTION_WEST];
  level = (std::tanh(level) + 1.0) / 2.0; // convert to 0.0..1.0
  if (level > 0.5) {
    target->setDirection(M_PI);
    return;
  }

  level = actions[Action::SET_DIRECTION_SOUTH];
  level = (std::tanh(level) + 1.0) / 2.0; // convert to 0.0..1.0
  if (level > 0.5) {
    target->setDirection(M_PI * 1.5);
    return;
  }

  level = actions[Action::SET_DIRECTION_WEST];
  level = (std::tanh(level) + 1.0) / 2.0; // convert to 0.0..1.0
  if (level > 0.5) {
    target->setDirection(M_PI * 2);
    return;
  }

  level = actions[Action::SET_DIRECTION];
  level = (std::tanh(level) + 1.0) / 2.0; // convert to 0.0..1.0
  auto direction = level * 2 * M_PI;
  target->setDirection(direction);
}

Genome Brain::getGenome() const {
  return genome;
}

NeuralNet Brain::getNeuralNet() const {
  return neuralNet;
}
