#define _USE_MATH_DEFINES
#include "CowardHunterStrategy.h"
#include "Bubble.h"
#include <cmath>
#include <algorithm>
#include <iterator>
using namespace agarai;
using namespace std;
////////////////////////////////////////////////////////////////////////////////
CowardHunterStrategy::CowardHunterStrategy(Bubble* target, bool huntBiggest)
	: HunterStrategy(target, huntBiggest)
{
	if(huntBiggest)
		target->setFieldOfViewColor(Color(94/256.0f, 225/256.0f, 7/256.0f));
	else
		target->setFieldOfViewColor(Color(8/256.0f, 115/256.0f, 12/256.0f));
}
//------------------------------------------------------------------------------
CowardHunterStrategy::~CowardHunterStrategy()
{
}
//------------------------------------------------------------------------------
void CowardHunterStrategy::decide(const DecisionContext& context)
{
	vector<Bubble*> threats;
	copy_if(context.visibleNeighbours.begin(), context.visibleNeighbours.end(), back_inserter(threats),
		[this](Bubble* b)
		{
			return b->isBigger(target);
		});

	if(!threats.empty())
	{
		//cout << target->getIdentifier() << "!!!" << endl;
		Coord2d slope;
		for(auto threat : threats)
		{
			Coord2d threatEffect = target->getPosition() - threat->getPosition();
			//normalize
			threatEffect.Y /= abs(threatEffect.X);
			threatEffect.X /= abs(threatEffect.X);
			//take into account size and distance
			threatEffect *= threat->getRadius() / target->getPosition().distance(threat->getPosition());

			slope += threatEffect;
		}
		
		//normalize
		slope.Y /= abs(slope.X);
		slope.X /= abs(slope.X);

		Coord2d edgeEffect;
		//get away from edges
		Coord2d currentPosition = target->getPosition();

		float distanceFromLeftEdge = currentPosition.X - context.worldLimits.left;
		if(distanceFromLeftEdge < target->getFieldOfViewRadius())
			edgeEffect.X += 1 * (target->getRadius() / distanceFromLeftEdge);

		float distanceFromRightEdge = context.worldLimits.right - currentPosition.X;
		if(distanceFromRightEdge < target->getFieldOfViewRadius())
			edgeEffect.X -= 1 * (target->getRadius() / distanceFromRightEdge);
		
		float distanceFromBottomEdge = currentPosition.Y - context.worldLimits.bottom;
		if(distanceFromBottomEdge < target->getFieldOfViewRadius())
			edgeEffect.Y += 1 * (target->getRadius() / distanceFromBottomEdge);

		float distanceFromTopEdge = context.worldLimits.top - currentPosition.Y;
		if(distanceFromTopEdge < target->getFieldOfViewRadius())
			edgeEffect.Y -= 1 * (target->getRadius() / distanceFromTopEdge);

		slope += edgeEffect;

		float dir = std::atan2(slope.Y, slope.X);
		if(dir < 0)
			dir += 2 * (float)M_PI;

		target->setDirection(dir);
		return;
	}

	HunterStrategy::decide(context);
}