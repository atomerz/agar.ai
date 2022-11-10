#pragma once
#include "agar/strategies/RandomStrategy.h"
////////////////////////////////////////////////////////////////////////////////
namespace agarai
{
	class HunterStrategy : public IBubbleControlStrategy
	{
	public:
		HunterStrategy(RenderableBubble* target, bool huntBiggest);
		~HunterStrategy();

		void decide(const DecisionContext& context);

	protected:
		RenderableBubble* target;
		bool huntBiggest;

	private:

		IBubbleControlStrategy* randomStrategy;

	};
}