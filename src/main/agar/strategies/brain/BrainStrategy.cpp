#include "agar/strategies/brain/BrainStrategy.h"
#include "agar/RenderableBubble.h"
#include <cmath>
#include <algorithm>
using namespace agarai;
////////////////////////////////////////////////////////////////////////////////
BrainStrategy::BrainStrategy(RenderableBubble* target)
{
	this->target = target;
	target->setFieldOfViewColor(Color(0.5f, 0.5f, 0));
}
//------------------------------------------------------------------------------
BrainStrategy::~BrainStrategy()
{
}
//------------------------------------------------------------------------------
void BrainStrategy::decide(const DecisionContext& context)
{
	float direction = randf() * 2 * (float)M_PI;
	target->setDirection(direction);
}