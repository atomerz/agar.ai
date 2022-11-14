#include "GameManager.h"

#include <algorithm>

#include "brain/Brain.h"

using namespace std;
using namespace agarai;
////////////////////////////////////////////////////////////////////////////////
GameManager::GameManager()
	: timeScale(1), initialBubbleMass(50), minFoodMass(5), maxFoodMass(10),
	noFoods(1000), noBubbles(200) {
}

GameManager::~GameManager() {
}

void GameManager::init(int* argc, char** argv, Dimension2D worldSize) {
	this->worldSize = worldSize;
	this->renderEngine = renderEngine;

	renderEngine.init(argc, argv, worldSize);

	Matrix4 pMatrix;
	pMatrix.ortho2D(0, (float)worldSize.width, 0, (float)worldSize.height);
	renderEngine.setProjectionMatrix(pMatrix);
	
	for(int i=0; i<noFoods; ++i) {
		RenderableBubble* food = new RenderableBubble(
			&renderEngine,
			randf(minFoodMass, maxFoodMass),
			generateRandomCoords());

		foods.push_back(food);
		renderEngine.addRenderableObject(food);
	}

	for(int i=0; i<noBubbles; ++i) {
		RenderableBubble* bubble = new RenderableBubble(&renderEngine, initialBubbleMass,
			generateRandomCoords(), std::move(Genome::random()));
		bubbles.push_back(bubble);
		renderEngine.addRenderableObject(bubble);
	}
}

void GameManager::run() {
	isRunning = true;
	thread trd([this]()	{
		clock_t lastUpdateTime = clock();
		clock_t lastReportTime = clock();
		while(isRunning) {
			std::vector<Genome> parentGenomes;
			std::transform(bubbles.begin(), bubbles.end(), back_inserter(parentGenomes), [](auto b){
			  return b->getBrain()->getGenome();
			});

			float elapsedSeconds = (clock() - lastUpdateTime) / (float)CLOCKS_PER_SEC;
			elapsedSeconds *= timeScale;
			for(auto bubble : bubbles) {
				DecisionContext context;
				context.me = bubble;
				context.worldLimits.left = 0;
				context.worldLimits.right = (float)worldSize.width;
				context.worldLimits.bottom = 0;
				context.worldLimits.top = (float)worldSize.height;

				for(auto neighbor : bubbles) {
					if(bubble != neighbor && bubble->isInFieldOfView(neighbor)) {
						context.visibleNeighbors.push_back(neighbor);
					}
				}

				for(auto food : foods) {
					if(bubble->isInFieldOfView(food)) {
						context.visibleNeighbors.push_back(food);
					}
				}
				
				sort(context.visibleNeighbors.begin(), context.visibleNeighbors.end(),
					[](auto b1, auto b2) {
						return b1->getMass() > b2->getMass();
					});

				bubble->update(
					elapsedSeconds,
					agarai::Rectangle(0, (float)worldSize.width, 0, (float)worldSize.height),
					context);
			}

			lastUpdateTime = clock();

			// check for collisions
			for(size_t i=0; i<bubbles.size(); ++i) {
				auto bubble = bubbles[i];

				// eat food
				for(auto food : foods) {
					if(bubble->encompass(food)) {
						bubble->eat(food);
						food->reset(randf(minFoodMass, maxFoodMass), generateRandomCoords());
					}
				}

				// eat other bubbles
				for(size_t j=i+1; j<bubbles.size(); ++j) {
					auto prey = bubbles[j];
					if(bubble->encompass(prey)) {
						bubble->eat(prey);
						prey->reset(initialBubbleMass, generateRandomCoords(), std::move(Genome::childGenome(parentGenomes)));
					}
				}
			}

			// sort by bubble size
			//TODO: render engine should also use the sorted list
			sort(bubbles.begin(), bubbles.end(), [](auto b1, auto b2) {
			  return b1->getMass() > b2->getMass();
			});

			// report summary
			auto sinceLastReportSeconds = (clock() - lastReportTime) / (float)CLOCKS_PER_SEC;
			if (sinceLastReportSeconds > 10) {
				cout << "Time Scale: " << timeScale << endl;
				
				cout << "Top Bubble: " << endl;
				cout << "  mass: " << bubbles[0]->getMass() << endl;
				cout << "  age: " << bubbles[0]->getAge() << endl;
				
				auto topBubble = bubbles[0]->getBrain();
				cout << "  generation: " << topBubble->getGenome().getGeneration() << endl;
				cout << "  genome:" << endl;
				topBubble->getGenome().print();
				cout << "  brain:" << endl;
				topBubble->getNeuralNet().printIGraphEdgeList();
				cout << endl << endl << endl;

				lastReportTime = clock();
			}

			this_thread::sleep_for(chrono::milliseconds((int)(10/timeScale)));
		};
	});
	updaterThread = &trd;

	glutMainLoop();
}

void GameManager::onRender() {
	renderEngine.onRender();
}

void GameManager::onWindowSizeChanged(int w, int h) {
	renderEngine.onWindowSizeChanged(w, h);
}

void GameManager::onTimer(int value) {
	renderEngine.onTimer(value);
}

void GameManager::onKeyPress(unsigned char key, int x, int y) {
}

void GameManager::onSpacialKeyPress(int key, int x, int y) {
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

void GameManager::onCleanup() {
	isRunning = false;
	updaterThread->join();

	for(auto bubble : bubbles)
		delete bubble;
	bubbles.clear();

	renderEngine.onCleanup();
}

Coord2d GameManager::generateRandomCoords() const {
	int margin = 10;
	Coord2d c;

	while(true)	{
		c.X = (float)(margin + (rand() % (worldSize.width - 2 * margin)));
		c.Y = (float)(margin + (rand() % (worldSize.height - 2 * margin)));
		for(auto bubble : bubbles) {
			if(bubble->encompass(c)) continue;
		}
		break;
	}

	return c;
}