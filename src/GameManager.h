#pragma once
#include "Bubble.h"
#include "graphics/RenderEngine.h"
#include "utils.h"
#include <vector>
#include <thread>
////////////////////////////////////////////////////////////////////////////////
namespace agarai
{
	class GameManager
	{
	public:
		GameManager();
		~GameManager();

		void init(int* argc, char** argv, Dimension2D worldSize);
		void run();

		void onRender();
		void onWindowSizeChanged(int w, int h);
		void onTimer(int value);
		void onKeyPress(unsigned char key, int x, int y);
		void onSpacialKeyPress(int key, int x, int y);
		void onCleanup();

	private:
		bool		isRunning;
		std::thread* updaterThread;
		RenderEngine renderEngine;
		Dimension2D worldSize;
		float		timeScale;

		const int	noFoods;
		const int	noBubbles;
		const float initialBubbleMass;
		const float minFoodMass;
		const float maxFoodMass;

		std::vector<Bubble*> bubbles;
		std::vector<Bubble*> foods;

	private:
		Coord2d generateRandomCoords() const;

	};
}