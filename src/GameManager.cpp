#include "GameManager.h"
#include "strategies/CowardHunterStrategy.h"
#include <algorithm>
using namespace std;
using namespace agarai;
////////////////////////////////////////////////////////////////////////////////
GameManager::GameManager()
	: timeScale(1), initialBubbleMass(100), minFoodMass(10), maxFoodMass(30),
	noFoods(500), noBubbles(100)
{
}
//------------------------------------------------------------------------------
GameManager::~GameManager()
{
}
//------------------------------------------------------------------------------
void GameManager::init(int* argc, char** argv, Dimension2D worldSize)
{
	this->worldSize = worldSize;
	this->renderEngine = renderEngine;

	renderEngine.init(argc, argv, worldSize);

	Matrix4 pMatrix;
	pMatrix.ortho2D(0, (float)worldSize.width, 0, (float)worldSize.height);
	renderEngine.setProjectionMatrix(pMatrix);
	
	for(int i=0; i<noFoods; ++i)
	{
		Bubble* food = new Bubble(
			&renderEngine,
			randf(minFoodMass, maxFoodMass),
			generateRandomCoords());

		foods.push_back(food);
		renderEngine.addRenderableObject(food);
	}

	for(int i=0; i<noBubbles; ++i)
	{
		Bubble* bubble = new Bubble(&renderEngine, initialBubbleMass, generateRandomCoords());
		IBubbleControlStrategy* strategy = nullptr;
		switch(i % 5)
		{
		case 0:
			strategy = new RandomStrategy(bubble);
			break;
		case 1:
			strategy = new HunterStrategy(bubble, false);
			break;
		case 2:
			strategy = new HunterStrategy(bubble, true);
			break;
		case 3:
			strategy = new CowardHunterStrategy(bubble, false);
			break;
		case 4:
			strategy = new CowardHunterStrategy(bubble, true);
			break;
		//TODO: this is for test, remove it when done.
		default:
			strategy = new CowardHunterStrategy(bubble, false);
			break;
		}
		
		bubble->setStrategy(strategy);

		bubbles.push_back(bubble);
		renderEngine.addRenderableObject(bubble);
	}
}
//------------------------------------------------------------------------------
void GameManager::run()
{
	isRunning = true;
	thread trd([this]()
	{
		clock_t lastUpdateTime = clock();
		while(isRunning)
		{
			float elapsedSeconds = (clock() - lastUpdateTime) / (float)CLOCKS_PER_SEC;
			elapsedSeconds *= timeScale;
			for(auto bubble : bubbles)
			{
				//create decision context
				DecisionContext context;
				context.worldLimits.left = 0;
				context.worldLimits.right = (float)worldSize.width;
				context.worldLimits.bottom = 0;
				context.worldLimits.top = (float)worldSize.height;

				for(auto neighbuor : bubbles)
				{
					if(bubble != neighbuor
						&& bubble->isInFieldOfView(neighbuor))
					{
						context.visibleNeighbours.push_back(neighbuor);
					}
				}

				for(auto food : foods)
					if(bubble->isInFieldOfView(food))
						context.visibleNeighbours.push_back(food);
				
			sort(context.visibleNeighbours.begin(), context.visibleNeighbours.end(),
				[](Bubble* b1, Bubble* b2)
				{
					return b1->getMass() > b2->getMass();
				});

				bubble->update(
					elapsedSeconds,
					agarai::Rectangle(0, (float)worldSize.width, 0, (float)worldSize.height),
					context);
			}

			lastUpdateTime = clock();

			// check for collisions
			for(size_t i=0; i<bubbles.size(); ++i)
			{
				Bubble* bubble = bubbles[i];

				//eat food
				for(auto food : foods)
				{
					if(bubble->encompass(food))
					{
						bubble->eat(food);
						food->reset(randf(minFoodMass, maxFoodMass), generateRandomCoords());
					}
				}

				//eat other bubbles
				for(size_t j=i+1; j<bubbles.size(); ++j)
				{
					Bubble* prey = bubbles[j];
					if(bubble->encompass(prey))
					{
						bubble->eat(prey);
						prey->reset(initialBubbleMass, generateRandomCoords());
					}
				}
			}

			// sort by bubble size
			//TODO: render engine should also use the sorted list
			sort(bubbles.begin(), bubbles.end(),
				[](Bubble* b1, Bubble* b2)
				{
					return b1->getMass() > b2->getMass();
				});

			cout << "Time Scale: " << timeScale 
				<< ". Best Bubble: " << bubbles[0]->getMass() << endl;

			this_thread::sleep_for(chrono::milliseconds((int)(10/timeScale)));
		};
	});
	updaterThread = &trd;

	glutMainLoop();
}
//------------------------------------------------------------------------------
void GameManager::onRender()
{
	renderEngine.onRender();
}
//------------------------------------------------------------------------------
void GameManager::onWindowSizeChanged(int w, int h)
{
	renderEngine.onWindowSizeChanged(w, h);
}
//------------------------------------------------------------------------------
void GameManager::onTimer(int value)
{
	renderEngine.onTimer(value);
}
//------------------------------------------------------------------------------
void GameManager::onKeyPress(unsigned char key, int x, int y)
{
}
//------------------------------------------------------------------------------
void GameManager::onSpacialKeyPress(int key, int x, int y)
{
	switch(key)
	{
	case GLUT_KEY_UP:
		timeScale *= 2;
		break;
	case GLUT_KEY_DOWN:
		timeScale /= 2;
		break;
	}
}
//------------------------------------------------------------------------------
void GameManager::onCleanup()
{
	isRunning = false;
	updaterThread->join();

	for(auto bubble : bubbles)
		delete bubble;
	bubbles.clear();

	renderEngine.onCleanup();
}
//------------------------------------------------------------------------------
Coord2d GameManager::generateRandomCoords() const
{
	int margin = 10;
	Coord2d c;

	while(true)
	{
		c.X = (float)(margin + (rand() % (worldSize.width - 2 * margin)));
		c.Y = (float)(margin + (rand() % (worldSize.height - 2 * margin)));
		for(auto bubble : bubbles)
			if(bubble->encompass(c))
				continue;
		break;
	}

	return c;
}