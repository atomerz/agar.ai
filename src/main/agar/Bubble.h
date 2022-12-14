#pragma once

#include <cstdint>
#include <memory>

#include "utils.h"
#include "ai/Genome.h"
#include "agar/brain/Brain.h"

namespace agarai {

class Bubble {
public:
	Bubble(float mass, float x, float y, std::unique_ptr<Genome> genome=nullptr);
	Bubble(float mass, Coord2d c, std::unique_ptr<Genome> genome=nullptr);
	virtual ~Bubble();

	virtual void reset(float mass, float x, float y, std::unique_ptr<Genome> genome=nullptr);
	virtual void reset(float mass, Coord2d c, std::unique_ptr<Genome> genome=nullptr);
	void eat(Bubble *b);

	/**
	 * @param elapsedTime seconds since last update.
	 */
	void update(float elapsedTime, agarai::Rectangle limits, DecisionContext context);
	/**
	 * Sets the direction in which the bubble is moving.
	 * @param dir Direction in radians in the range [0, 2*pi).
	 */
	void setDirection(float dir);
	virtual void setBrain(std::unique_ptr<Brain>&& brain);

	Coord2d getPosition() const;
	float getRadius() const;
	float getFieldOfViewRadius() const;
	float getMass() const;
	float getCurrentSpeed() const;
  	Brain* getBrain() const;
  	float getAge() const;
  	float getSinceLastEat() const;

	bool encompass(Coord2d c) const;
	bool encompass(Bubble *b) const;
	/**
	 * Returns whether or not this object can see <code>b</code>.
	 */
	bool isInFieldOfView(Bubble *b) const;
	/**
	 * Returns whether or not this bubble can eat the target bubble.
	 * This method compares sizes only.
	 */
	bool isBigger(Bubble *b) const;

	std::string toString() const;

protected:
  bool isDead() const;
  float getDirection() const;

private:
	const static float decayRate;

	float surface;

	Coord2d position;
	float moveDirection;

	std::unique_ptr<Brain> brain;
	
	bool _isDead;

	float age;
	float sinceLastEat;

private:
	void die();

	void updatePosition(float elapsedTime, agarai::Rectangle limits);
	void updateSurface(float elapsedTime);
};

}
