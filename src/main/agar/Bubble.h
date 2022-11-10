#pragma once
#include "agar/IBubbleControlStrategy.h"
#include "utils.h"
#include <cstdint>
////////////////////////////////////////////////////////////////////////////////
namespace agarai
{
	class Bubble {
	public:
		Bubble(float mass, float x, float y);
		Bubble(float mass, Coord2d c);
		virtual ~Bubble();

		void reset(float mass, float x, float y);
		void reset(float mass, Coord2d c);
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
		void setStrategy(std::unique_ptr<IBubbleControlStrategy>&& strategy);

		size_t getIdentifier() const;
		Coord2d getPosition() const;
		float getRadius() const;
		float getFieldOfViewRadius() const;
		float getMass() const;
		float getCurrentSpeed() const;

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

	private:
		const static float decayRate;

		size_t identifier;
		float surface;

		Coord2d position;
		float moveDirection;

		std::unique_ptr<IBubbleControlStrategy> controlStrategy;

		bool isDead;

	private:
		void die();

		void updatePosition(float elapsedTime, agarai::Rectangle limits);
		void updateSurface(float elapsedTime);
	};
}