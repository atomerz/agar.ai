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
Bubble::Bubble(float mass, float x, float y) {
	reset(mass, x, y);
	controlStrategy = nullptr;
}
//------------------------------------------------------------------------------
Bubble::Bubble(float mass, Coord2d c) {
	reset(mass, c);
}
//------------------------------------------------------------------------------
Bubble::~Bubble() {
}
//------------------------------------------------------------------------------
void Bubble::reset(float mass, float x, float y) {
	isDead = false;
	moveDirection = -1;
	
	surface = mass;
	position.X = x;
	position.Y = y;
}
//------------------------------------------------------------------------------
void Bubble::reset(float mass, Coord2d c) {
	reset(mass, c.X, c.Y);
}
//------------------------------------------------------------------------------
bool Bubble::encompass(Coord2d c) const {
	if(isDead)
		return false;

	return position.distance(c) < getRadius();
}
//------------------------------------------------------------------------------
bool Bubble::encompass(Bubble* target) const {
	if(isDead
		|| target->isDead
		|| !isBigger(target))
		return false;

	return position.distance(target->position) < getRadius() - 1;
}
//------------------------------------------------------------------------------
bool Bubble::isInFieldOfView(Bubble* b) const {
	if(isDead || b->isDead)
		return false;

	return (position.distance(b->position) - b->getRadius()) < getFieldOfViewRadius();
}
//------------------------------------------------------------------------------
bool Bubble::isBigger(Bubble* b) const {
	assert(!isDead && !b->isDead);

	return getRadius() >= b->getRadius() + 1;
}
//------------------------------------------------------------------------------
void Bubble::eat(Bubble* target) {
	assert(!isDead && !target->isDead
		&& this->isBigger(target)
		&& controlStrategy != nullptr);

	surface += target->surface;
	target->die();
}
//------------------------------------------------------------------------------
void Bubble::die() {
	assert(!isDead);

	isDead = true;
}
//------------------------------------------------------------------------------
size_t Bubble::getIdentifier() const {
	return identifier;
}
//------------------------------------------------------------------------------
Coord2d Bubble::getPosition() const {
	return position;
}
//------------------------------------------------------------------------------
float Bubble::getRadius() const {
	if(isDead)
		return 0;

	return sqrtf(surface / (float)M_PI);
}
//------------------------------------------------------------------------------
float Bubble::getFieldOfViewRadius() const {
	float r = getRadius();
	float coef = 1 - (r / (r + 10)); // a function that slowly goes from 1 to 0
	return max(2.5f * r, 10.0f * r * coef);
}
//------------------------------------------------------------------------------
float Bubble::getMass() const {
	if(isDead)
		return 0;

	return (float)surface;
}
//------------------------------------------------------------------------------
void Bubble::update(float elapsedTime, agarai::Rectangle limits, DecisionContext context) {
	if(isDead || controlStrategy == NULL)
		return;

	controlStrategy->decide(context);

	updatePosition(elapsedTime, limits);
	updateSurface(elapsedTime);
}
//------------------------------------------------------------------------------
void Bubble::updatePosition(float elapsedTime, agarai::Rectangle limits) {
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
void Bubble::updateSurface(float elapsedTime) {
	if(isDead || controlStrategy == NULL)
		return;
	
	if(surface > 300.0f)
		surface -= decayRate * elapsedTime * powf(surface - 300, 2);
}
//------------------------------------------------------------------------------
float Bubble::getCurrentSpeed() const {
	if(isDead)
		return 0;

	return max(1.0f, 1000 / sqrt(surface));
}
//------------------------------------------------------------------------------
void Bubble::setDirection(float dir) {
	moveDirection = dir;
}
//------------------------------------------------------------------------------
void Bubble::setStrategy(std::unique_ptr<IBubbleControlStrategy>&& strategy) {
	assert(strategy.get() != nullptr);
	this->controlStrategy = std::move(strategy);
}