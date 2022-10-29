#pragma once
#include "strategies/HunterStrategy.h"
////////////////////////////////////////////////////////////////////////////////
namespace agarai
{
	class CowardHunterStrategy : public HunterStrategy
	{
	public:
		CowardHunterStrategy(Bubble* target, bool huntBiggest);
		~CowardHunterStrategy();

		void decide(const DecisionContext& context);
	};
}