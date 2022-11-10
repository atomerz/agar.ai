#pragma once
#include "agar/strategies/HunterStrategy.h"
////////////////////////////////////////////////////////////////////////////////
namespace agarai
{
	class CowardHunterStrategy : public HunterStrategy
	{
	public:
		CowardHunterStrategy(RenderableBubble* target, bool huntBiggest);
		~CowardHunterStrategy();

		void decide(const DecisionContext& context);
	};
}