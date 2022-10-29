#define _USE_MATH_DEFINES
#include <cmath>
#include <sstream>
#include <cassert>
#include "Bubble.h"
#include "graphics/Matrices.h"
using namespace std;
using namespace agarai;
////////////////////////////////////////////////////////////////////////////////
const float Bubble::decayRate = 0.00001f;
////////////////////////////////////////////////////////////////////////////////
Bubble::Bubble(IRenderEngine* renderEngine, float mass, float x, float y)
	: fovColor(0.3f, 0.3f, 0.3f)
{
	init(renderEngine);	
	reset(mass, x, y);
}
//------------------------------------------------------------------------------
Bubble::Bubble(IRenderEngine* renderEngine, float mass, Coord2d c)
{
	init(renderEngine);
	reset(mass, c);
}
//------------------------------------------------------------------------------
Bubble::~Bubble()
{
	if(controlStrategy != NULL)
		delete controlStrategy;
	
	renderEngine->destroyVBO(lineVBO);
	renderEngine->destroyVBO(circleVBO);
}
//------------------------------------------------------------------------------
void Bubble::init(IRenderEngine* renderEngine)
{
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
	hash<Bubble*> hash_fn;
	identifier = hash_fn(this);
}
//------------------------------------------------------------------------------
void Bubble::reset(float mass, float x, float y)
{
	isDead = false;
	moveDirection = -1;
	
	color = Color::randomize();

	surface = mass;
	position.X = x;
	position.Y = y;
}
//------------------------------------------------------------------------------
void Bubble::reset(float mass, Coord2d c)
{
	reset(mass, c.X, c.Y);
}
//------------------------------------------------------------------------------
bool Bubble::encompass(Coord2d c) const
{
	if(isDead)
		return false;

	return position.distance(c) < getRadius();
}
//------------------------------------------------------------------------------
bool Bubble::encompass(Bubble* target) const
{
	if(isDead
		|| target->isDead
		|| !isBigger(target))
		return false;

	return position.distance(target->position) < getRadius() - 1;
}
//------------------------------------------------------------------------------
bool Bubble::isInFieldOfView(Bubble* b) const
{
	if(isDead || b->isDead)
		return false;

	return (position.distance(b->position) - b->getRadius()) < getFieldOfViewRadius();
}
//------------------------------------------------------------------------------
bool Bubble::isBigger(Bubble* b) const
{
	assert(!isDead && !b->isDead);

	return getRadius() >= b->getRadius() + 1;
}
//------------------------------------------------------------------------------
void Bubble::eat(Bubble* target)
{
	assert(!isDead && !target->isDead
		&& this->isBigger(target)
		&& controlStrategy != nullptr);

	surface += target->surface;
	target->die();
}
//------------------------------------------------------------------------------
void Bubble::die()
{
	assert(!isDead);

	isDead = true;
}
//------------------------------------------------------------------------------
size_t Bubble::getIdentifier() const
{
	return identifier;
}
//------------------------------------------------------------------------------
Coord2d Bubble::getPosition() const
{
	return position;
}
//------------------------------------------------------------------------------
float Bubble::getRadius() const
{
	if(isDead)
		return 0;

	return sqrtf(surface / (float)M_PI);
}
//------------------------------------------------------------------------------
float Bubble::getFieldOfViewRadius() const
{
	float r = getRadius();
	float coef = 1 - (r / (r + 10)); // a function that slowly goes from 1 to 0
	return max(2.5f * r, 10.0f * r * coef);
}
//------------------------------------------------------------------------------
float Bubble::getMass() const
{
	if(isDead)
		return 0;

	return (float)surface;
}
//------------------------------------------------------------------------------
void Bubble::render() 
{
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
void Bubble::update(float elapsedTime, agarai::Rectangle limits, DecisionContext context)
{
	if(isDead || controlStrategy == NULL)
		return;

	controlStrategy->decide(context);

	updatePosition(elapsedTime, limits);
	updateSurface(elapsedTime);
}
//------------------------------------------------------------------------------
void Bubble::updatePosition(float elapsedTime, agarai::Rectangle limits)
{
	if(isDead || moveDirection < 0)
		return;

	position.X += cosf(moveDirection) * getCurrentSpeed() * elapsedTime;
	position.Y += sinf(moveDirection) * getCurrentSpeed() * elapsedTime;

	float margin = getRadius() / 2;
	position.X = max(limits.left + margin, position.X);
	position.X = min(limits.right - margin, position.X);
	position.Y = max(limits.bottom +  margin, position.Y);
	position.Y = min(limits.top - margin, position.Y);	
}
//------------------------------------------------------------------------------
void Bubble::updateSurface(float elapsedTime)
{
	if(isDead || controlStrategy == NULL)
		return;
	
	if(surface > 300.0f)
		surface -= decayRate * elapsedTime * powf(surface - 300, 2);
}
//------------------------------------------------------------------------------
float Bubble::getCurrentSpeed() const
{
	if(isDead)
		return 0;

	return max(1.0f, 1000 / sqrt(surface));
}
//------------------------------------------------------------------------------
void Bubble::setDirection(float dir)
{
	moveDirection = dir;
}
//------------------------------------------------------------------------------
void Bubble::setStrategy(IBubbleControlStrategy* strategy)
{
	assert(strategy != nullptr);
	this->controlStrategy = strategy;
}
//------------------------------------------------------------------------------
void Bubble::setFieldOfViewColor(Color color)
{
	fovColor = color;
}