#pragma once

#include "agar/IBubbleControlStrategy.h"
#include "ai/NeuralNet.h"
#include "ai/Sensor.h"

#include <vector>
////////////////////////////////////////////////////////////////////////////////
namespace agarai
{
	class RenderableBubble;
	////////////////////////////////////////////////////////////////////////////////
	class BrainStrategy : public IBubbleControlStrategy
	{
	public:
		BrainStrategy(RenderableBubble* target);
		~BrainStrategy();

		void decide(const DecisionContext& context);

	private:
		RenderableBubble* target;
        Genome genome;
        NeuralNet neuralNet;

	};
}