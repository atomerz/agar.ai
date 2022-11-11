#include "agar/strategies/brain/BrainStrategy.h"
#include "agar/RenderableBubble.h"
#include "agar/strategies/brain/SensorImpl.h"

using namespace agarai;

// TODO: strategy should only need Bubble*
BrainStrategy::BrainStrategy(RenderableBubble* target, Genome genome): neuralNet(genome) {
  this->target = target;
  target->setFieldOfViewColor(Color(0.5f, 0.5f, 0));
}

BrainStrategy::~BrainStrategy() {
}

void BrainStrategy::decide(const DecisionContext& context) {
  auto sensor = SensorImpl(context);
  auto actions = neuralNet.feedForward(&sensor);

  auto level = actions[Action::SET_DIRECTION];
  level = (std::tanh(level) + 1.0) / 2.0; // convert to 0.0..1.0
  auto direction = level * M_PI;
  target->setDirection(direction);
}
