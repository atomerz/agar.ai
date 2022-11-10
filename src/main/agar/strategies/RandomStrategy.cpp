#define _USE_MATH_DEFINES
#include <cmath>
#include "agar/strategies/RandomStrategy.h"
#include "agar/RenderableBubble.h"
using namespace agarai;
////////////////////////////////////////////////////////////////////////////////
const int RandomStrategy::decisionFrequency = 50;
////////////////////////////////////////////////////////////////////////////////
RandomStrategy::RandomStrategy(RenderableBubble* target)
{
	this->target = target;
	target->setFieldOfViewColor(Color(0.5f, 0.5f, 0));
}
//------------------------------------------------------------------------------
RandomStrategy::~RandomStrategy()
{
}
//------------------------------------------------------------------------------
void RandomStrategy::decide(const DecisionContext& context)
{
	if(std::rand() % decisionFrequency)
		return;

	float direction = randf() * 2 * (float)M_PI;
	target->setDirection(direction);
}