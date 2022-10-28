#pragma once
#include "RandomStrategy.h"
////////////////////////////////////////////////////////////////////////////////
namespace agarai
{
	class HunterStrategy : public IBubbleControlStrategy
	{
	public:
		HunterStrategy(Bubble* target, bool huntBiggest);
		~HunterStrategy();

		void decide(const DecisionContext& context);

	protected:
		Bubble* target;
		bool huntBiggest;

	private:

		IBubbleControlStrategy* randomStrategy;

	};
}