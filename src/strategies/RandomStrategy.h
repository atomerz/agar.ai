#pragma once
#include "IBubbleControlStrategy.h"
#include <vector>
namespace agarai
{
	class Bubble;
	////////////////////////////////////////////////////////////////////////////////
	class RandomStrategy : public IBubbleControlStrategy
	{
	public:
		RandomStrategy(Bubble* target);
		~RandomStrategy();

		void decide(const DecisionContext& context);

	private:
		Bubble* target;
		static const int decisionFrequency;

	};
}