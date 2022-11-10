#define _USE_MATH_DEFINES
#include <cmath>
#include <sstream>
#include <cassert>
#include "RenderableBubble.h"
#include "graphics/Matrices.h"
using namespace std;
using namespace agarai;
////////////////////////////////////////////////////////////////////////////////
const float RenderableBubble::decayRate = 0.00001f;
////////////////////////////////////////////////////////////////////////////////
RenderableBubble::RenderableBubble(IRenderEngine* renderEngine, float mass, float x, float y)
  : Bubble(mass, x, y), fovColor(0.3f, 0.3f, 0.3f) {
	init(renderEngine);
}
//------------------------------------------------------------------------------
RenderableBubble::RenderableBubble(IRenderEngine* renderEngine, float mass, Coord2d c)
	: Bubble(mass, c), fovColor(0.3f, 0.3f, 0.3f) {
	init(renderEngine);
}
//------------------------------------------------------------------------------
RenderableBubble::~RenderableBubble() {	
	renderEngine->destroyVBO(lineVBO);
	renderEngine->destroyVBO(circleVBO);
}
//------------------------------------------------------------------------------
void RenderableBubble::init(IRenderEngine* renderEngine) {
	this->renderEngine = renderEngine;

	lineVBO = renderEngine->createVBO();
	Vertex lineVertices[2] = { Vertex(0, 0), Vertex(1, 0) };
	lineVBO.setBufferData(lineVertices, 2, VertexBufferObject::Usage::VBO_USAGE_STAIC_DRAW);

	circleVBO = renderEngine->createVBO();
	Vertex circleVertices[100];
	float arc = (float)(2 * M_PI / 100);
	for(int i=0; i<100; ++i)
		circleVertices[i].setXYZW(cosf(i * arc), sinf(i * arc), 0);
	circleVBO.setBufferData(circleVertices, 100, VertexBufferObject::Usage::VBO_USAGE_STAIC_DRAW);

	controlStrategy = NULL;
	hash<RenderableBubble*> hash_fn;
	identifier = hash_fn(this);
}
//------------------------------------------------------------------------------
void RenderableBubble::reset(float mass, float x, float y) {
	Bubble::reset(mass, x, y);
	color = Color::randomize();
}
//------------------------------------------------------------------------------
void RenderableBubble::reset(float mass, Coord2d c) {
	reset(mass, c.X, c.Y);
}
//------------------------------------------------------------------------------
void RenderableBubble::render() {
	if(isDead)
		return;

	Matrix4 mvMatrix;
	float bubbleScale = getRadius();
	mvMatrix.identity();
	mvMatrix.scale(bubbleScale, bubbleScale, 1);
	mvMatrix.translate(position.X, position.Y, 0);
	renderEngine->setModelViewMatrix(mvMatrix);

	renderEngine->setColor(color);

	renderEngine->render(circleVBO, IRenderEngine::RenderMode::RENDER_MODE_POLYGON);

	if(moveDirection >= 0)
	{
		mvMatrix.identity();
		mvMatrix.scale(getFieldOfViewRadius(), getFieldOfViewRadius(), 1);
		mvMatrix.rotate(moveDirection * 180 / (float)M_PI, 0, 0, 1);
		mvMatrix.translate(position.X, position.Y, 0);
		renderEngine->setModelViewMatrix(mvMatrix);

		renderEngine->setColor(Color(1, 1, 1));

		renderEngine->render(lineVBO, IRenderEngine::RenderMode::RENDER_MODE_LINES);
	}

	if(controlStrategy != nullptr)
	{
		float fovScale = getFieldOfViewRadius();
		mvMatrix.identity();
		mvMatrix.scale(fovScale, fovScale, 1);
		mvMatrix.translate(position.X, position.Y, 0);
		renderEngine->setModelViewMatrix(mvMatrix);

		renderEngine->setColor(fovColor);

		renderEngine->render(circleVBO, IRenderEngine::RenderMode::RENDER_MODE_LINE_LOOP);
	}
}
//------------------------------------------------------------------------------
void RenderableBubble::setFieldOfViewColor(Color color)
{
	fovColor = color;
}