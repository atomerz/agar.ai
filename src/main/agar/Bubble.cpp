#include "Bubble.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <sstream>
#include <cassert>

#include "graphics/Matrices.h"

using namespace std;
using namespace agarai;

const float Bubble::decayRate = 0.00001f;

Bubble::Bubble(float mass, float x, float y, std::unique_ptr<Genome> genome) {
	reset(mass, x, y, std::move(genome));
}

Bubble::Bubble(float mass, Coord2d c, std::unique_ptr<Genome> genome) {
	reset(mass, c, std::move(genome));
}

Bubble::~Bubble() {
}

void Bubble::reset(float mass, float x, float y, std::unique_ptr<Genome> genome) {
  _isDead = false;
  moveDirection = -1;

  surface = mass;
  position.X = x;
  position.Y = y;

  age = 0;
  sinceLastEat = 0;

  if (genome) {
    brain.reset(new Brain(this, *genome));
  } else {
    brain.reset();
  }
}

void Bubble::reset(float mass, Coord2d c, std::unique_ptr<Genome> genome) {
	reset(mass, c.X, c.Y, std::move(genome));
}

bool Bubble::encompass(Coord2d c) const {
	if(_isDead) return false;

	return position.distance(c) < getRadius();
}

bool Bubble::encompass(Bubble* target) const {
	if(_isDead
		|| target->_isDead
		|| !isBigger(target))
		return false;

	return position.distance(target->position) < getRadius() - 1;
}

bool Bubble::isInFieldOfView(Bubble* b) const {
	if(_isDead || b->_isDead) return false;

	return (position.distance(b->position) - b->getRadius()) < getFieldOfViewRadius();
}

bool Bubble::isBigger(Bubble* b) const {
	assert(!_isDead && !b->_isDead);

	return getRadius() >= b->getRadius() + 1;
}

std::string Bubble::toString() const {
	stringstream sstr;
	sstr << "{\n";
	sstr << "  age: " << getAge() << endl;
	sstr << "  mass: " << getMass() << endl;
	sstr << "  sinceLastEat: " << getSinceLastEat() << endl;
	if (brain) {
	sstr << "  brain:\n";
	sstr << "    generation: " << brain->getGenome().getGeneration() << endl;
	sstr << "    graph:\n";
	brain->getNeuralNet().printIGraphEdgeList(sstr);
	}
	sstr << "}\n";

	return sstr.str();
}

void Bubble::eat(Bubble* target) {
	assert(!_isDead && !target->_isDead
		&& this->isBigger(target)
		&& brain != nullptr);

	surface += target->surface;
	target->die();

	sinceLastEat = 0;
}

void Bubble::die() {
	assert(!_isDead);

	_isDead = true;
}

bool Bubble::isDead() const {
  return _isDead;
}

float Bubble::getDirection() const {
  return moveDirection;
}

Brain* Bubble::getBrain() const {
  return brain.get();
}

float Bubble::getAge() const {
  return age;
}

float Bubble::getSinceLastEat() const {
  return sinceLastEat;
}

Coord2d Bubble::getPosition() const {
	return position;
}

float Bubble::getRadius() const {
	if(_isDead) return 0;

	return sqrtf(surface / (float)M_PI);
}

float Bubble::getFieldOfViewRadius() const {
	float r = getRadius();
	float coef = 1 - (r / (r + 10)); // a function that slowly goes from 1 to 0
	return max(2.5f * r, 10.0f * r * coef);
}

float Bubble::getMass() const {
	if(_isDead) return 0;

	return (float)surface;
}

void Bubble::update(float elapsedTime, agarai::Rectangle limits, DecisionContext context) {
	if(_isDead || brain == nullptr) return;

	age += elapsedTime;
	sinceLastEat += elapsedTime;

	brain->decide(context);

	updatePosition(elapsedTime, limits);
	updateSurface(elapsedTime);
}

void Bubble::updatePosition(float elapsedTime, agarai::Rectangle limits) {
	if(_isDead || moveDirection < 0)
		return;

	position.X += cosf(moveDirection) * getCurrentSpeed() * elapsedTime;
	position.Y += sinf(moveDirection) * getCurrentSpeed() * elapsedTime;

	float margin = getRadius() / 2;
	position.X = max(limits.left + margin, position.X);
	position.X = min(limits.right - margin, position.X);
	position.Y = max(limits.bottom +  margin, position.Y);
	position.Y = min(limits.top - margin, position.Y);	
}

void Bubble::updateSurface(float elapsedTime) {
	if(_isDead || brain == nullptr)
		return;
	
	if(surface > 300.0f) surface -= decayRate * elapsedTime * powf(surface - 300, 2);
}

float Bubble::getCurrentSpeed() const {
	if(_isDead)
		return 0;

	return max(1.0f, 1000 / sqrt(surface));
}

void Bubble::setDirection(float dir) {
	moveDirection = dir;
}

void Bubble::setBrain(std::unique_ptr<Brain>&& brain) {
	assert(this->brain == nullptr);
	this->brain = std::move(brain);
}