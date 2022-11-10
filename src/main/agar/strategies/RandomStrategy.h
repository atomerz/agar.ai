#pragma once
#include "agar/IBubbleControlStrategy.h"
#include <vector>
////////////////////////////////////////////////////////////////////////////////
namespace agarai
{
	class RenderableBubble;
	////////////////////////////////////////////////////////////////////////////////
	class RandomStrategy : public IBubbleControlStrategy
	{
	public:
		RandomStrategy(RenderableBubble* target);
		~RandomStrategy();

		void decide(const DecisionContext& context);

	private:
		RenderableBubble* target;
		static const int decisionFrequency;

	};
}