#pragma once
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "graphics/IRenderEngine.h"
#include "graphics/IRenderableObject.h"
#include "IBubbleControlStrategy.h"
#include "utils.h"
#include <cstdint>
////////////////////////////////////////////////////////////////////////////////
namespace agarai
{
	class Bubble : public IRenderableObject
	{
	public:
		Bubble(IRenderEngine* renderEngine, float mass, float x, float y);
		Bubble(IRenderEngine* renderEngine, float mass, Coord2d c);
		~Bubble();

		void reset(float mass, float x, float y);
		void reset(float mass, Coord2d c);
		void eat(Bubble* b);

		void render();
		/**
		* @param elapsedTime seconds since last update.
		*/
		void update(float elapsedTime, agarai::Rectangle limits, DecisionContext context);
		/**
		* Sets the direction in which the bubble is moving.
		* @param dir Direction in radians in the range [0, 2*pi).
		*/
		void setDirection(float dir);
		void setStrategy(IBubbleControlStrategy* strategy);
		void setFieldOfViewColor(Color color);

		size_t getIdentifier() const;
		Coord2d getPosition() const;
		float getRadius() const;
		float getFieldOfViewRadius() const;
		float getMass() const;
		float getCurrentSpeed() const;
		
		bool encompass(Coord2d c) const;
		bool encompass(Bubble* b) const;
		/**
		* Returns whether or not this object can see <code>b</code>.
		*/
		bool isInFieldOfView(Bubble* b) const;
		/**
		* Returns whether or not this bubble can eat the target bubble.
		* This method compares sizes only.
		*/
		bool isBigger(Bubble* b) const;

	private:
		const static float		decayRate;

		IRenderEngine*			renderEngine;
		//TODO: these can be static or placed somewhere to be created only once for all bubbles
		VertexBufferObject		lineVBO;
		VertexBufferObject		circleVBO;

		size_t					identifier;
		float					surface;
		Color					color;
		Color					fovColor;

		Coord2d					position;
		float					moveDirection;

		IBubbleControlStrategy* controlStrategy;

		bool					isDead;

	private:
		void					init(IRenderEngine* renderEngine);
		void					die();

		void					updatePosition(float elapsedTime, agarai::Rectangle limits);
		void					updateSurface(float elapsedTime);

	};
}