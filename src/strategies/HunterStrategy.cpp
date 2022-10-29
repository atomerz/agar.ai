#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include "strategies/HunterStrategy.h"
#include "Bubble.h"
using namespace agarai;
using namespace std;
////////////////////////////////////////////////////////////////////////////////
HunterStrategy::HunterStrategy(Bubble* target, bool huntBiggest)
{
	this->target = target;
	this->huntBiggest = huntBiggest;
	this->randomStrategy = new RandomStrategy(target);

	if(huntBiggest)
		target->setFieldOfViewColor(Color(0.0f, 0.0f, 1.0f));
	else
		target->setFieldOfViewColor(Color(1.0f, 0.0f, 1.0f));
}
//------------------------------------------------------------------------------
HunterStrategy::~HunterStrategy()
{
}
//------------------------------------------------------------------------------
bool fn(Bubble* b1, Bubble* b2)
{
	return true;
}
void HunterStrategy::decide(const DecisionContext& context)
{
	vector<Bubble*> preys;
	copy_if(context.visibleNeighbours.begin(), context.visibleNeighbours.end(), back_inserter(preys),
		[this](Bubble* b)
		{
			return target->isBigger(b);
		});

	if(preys.empty())
	{
		//cout << target->getIdentifier() << " --> ?" << ". n= " << context.visibleNeighbours.size() << endl;

		randomStrategy->decide(context);
		return;
	}

	//cout << target->getIdentifier() << "->" << prey->getIdentifier() << ":"
	//	<< (huntBiggest ? "b" : "s") << endl;
	
	sort(preys.begin(), preys.end(),
		[this](Bubble* b1, Bubble* b2)
		{
			if(huntBiggest)
			{
				float b1Value = b1->getMass() - target->getPosition().distance(b1->getPosition());
				float b2Value = b2->getMass() - target->getPosition().distance(b2->getPosition());
				return b1Value > b2Value;
			}
			
			float b1Value = -target->getPosition().distance(b1->getPosition());
			float b2Value = -target->getPosition().distance(b2->getPosition());
			return b1Value > b2Value;
		});

	Bubble* prey = preys.front();
	Coord2d slope = prey->getPosition() - target->getPosition();
	float dir = std::atan2(slope.Y, slope.X);
	if(dir < 0)
		dir += 2 * (float)M_PI;

	target->setDirection(dir);
}