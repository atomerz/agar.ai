#include "RenderableBubble.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <sstream>
#include <cassert>

#include "graphics/Matrices.h"

using namespace std;
using namespace agarai;

RenderableBubble::RenderableBubble(IRenderEngine* renderEngine, float mass, float x, float y, Genome genome)
  : Bubble(mass, x, y, genome), fovColor(0.3f, 0.3f, 0.3f) {
  init(renderEngine);
  reset(mass, x, y, genome);
}

RenderableBubble::RenderableBubble(IRenderEngine* renderEngine, float mass, Coord2d c, Genome genome)
	: Bubble(mass, c, genome), fovColor(0.3f, 0.3f, 0.3f) {
  init(renderEngine);
  reset(mass, c, genome);
}

RenderableBubble::~RenderableBubble() {	
	renderEngine->destroyVBO(lineVBO);
	renderEngine->destroyVBO(circleVBO);
}

void RenderableBubble::init(IRenderEngine* renderEngine) {
	this->renderEngine = renderEngine;

	lineVBO = renderEngine->createVBO();
	Vertex lineVertices[2] = { Vertex(0, 0), Vertex(1, 0) };
	lineVBO.setBufferData(lineVertices, 2, VertexBufferObject::Usage::VBO_USAGE_STATIC_DRAW);

	circleVBO = renderEngine->createVBO();
	Vertex circleVertices[100];
	float arc = (float)(2 * M_PI / 100);
	for(int i=0; i<100; ++i) {
		circleVertices[i].setXYZW(cosf(i * arc), sinf(i * arc), 0);
	}
	circleVBO.setBufferData(circleVertices, 100, VertexBufferObject::Usage::VBO_USAGE_STATIC_DRAW);
}

static Color makeGeneticColor(const Genome &genome) {
  if (genome.empty()) return Color::randomize();

  uint8_t simple = ((genome.size() & 1)
    | ((genome.front().sourceType)    << 1)
    | ((genome.back().sourceType)     << 2)
    | ((genome.front().sinkType)      << 3)
    | ((genome.back().sinkType)       << 4)
    | ((genome.front().sourceNum & 1) << 5)
    | ((genome.front().sinkNum & 1)   << 6)
    | ((genome.back().sourceNum & 1)  << 7));

  auto r = simple;
  auto g = ((simple & 0x1f) << 3);
  auto b = ((simple & 7)    << 5);

  constexpr uint8_t maxColorVal = 0xb0;
  constexpr uint8_t maxLumaVal = 0xb0;
  auto rgbToLuma = [](uint8_t r, uint8_t g, uint8_t b) { return (r+r+r+b+g+g+g+g) / 8; };
  // Prevent color mappings to very bright colors (hard to see):
  if (rgbToLuma(r, g, b) > maxLumaVal) {
    if (r > maxColorVal) r %= maxColorVal;
    if (g > maxColorVal) g %= maxColorVal;
    if (b > maxColorVal) b %= maxColorVal;
  }

  return Color(r/255.0, g/255.0, b/255.0);
}

void RenderableBubble::reset(float mass, float x, float y, Genome genome) {
	Bubble::reset(mass, x, y, genome);
	color = makeGeneticColor(genome);
}

void RenderableBubble::reset(float mass, Coord2d c, Genome genome) {
	reset(mass, c.X, c.Y, genome);
}

void RenderableBubble::setBrain(std::unique_ptr<Brain>&& brain) {
  Bubble::setBrain(std::move(brain));
}

void RenderableBubble::render() {
	if(isDead()) return;

	auto position = getPosition();

	Matrix4 mvMatrix;
	float bubbleScale = getRadius();
	mvMatrix.identity();
	mvMatrix.scale(bubbleScale, bubbleScale, 1);
	mvMatrix.translate(position.X, position.Y, 0);
	renderEngine->setModelViewMatrix(mvMatrix);

	renderEngine->setColor(color);

	renderEngine->render(circleVBO, IRenderEngine::RenderMode::RENDER_MODE_POLYGON);

	auto direction = getDirection();
	if(direction >= 0) {
		mvMatrix.identity();
		mvMatrix.scale(getFieldOfViewRadius(), getFieldOfViewRadius(), 1);
		mvMatrix.rotate(direction * 180 / (float)M_PI, 0, 0, 1);
		mvMatrix.translate(position.X, position.Y, 0);
		renderEngine->setModelViewMatrix(mvMatrix);

		renderEngine->setColor(Color(1, 1, 1));

		renderEngine->render(lineVBO, IRenderEngine::RenderMode::RENDER_MODE_LINES);
	}

	if(getBrain() != nullptr) {
		float fovScale = getFieldOfViewRadius();
		mvMatrix.identity();
		mvMatrix.scale(fovScale, fovScale, 1);
		mvMatrix.translate(position.X, position.Y, 0);
		renderEngine->setModelViewMatrix(mvMatrix);

		renderEngine->setColor(fovColor);

		renderEngine->render(circleVBO, IRenderEngine::RenderMode::RENDER_MODE_LINE_LOOP);
	}
}

void RenderableBubble::setFieldOfViewColor(Color color) {
	fovColor = color;
}