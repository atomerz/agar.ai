#pragma once
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "graphics/IRenderEngine.h"
#include "graphics/IRenderableObject.h"
#include "agar/Bubble.h"
#include "agar/IBubbleControlStrategy.h"
#include "utils.h"

#include <cstdint>
////////////////////////////////////////////////////////////////////////////////
namespace agarai
{
	class RenderableBubble : public Bubble, public IRenderableObject {
	public:
		RenderableBubble(IRenderEngine* renderEngine, float mass, float x, float y);
		RenderableBubble(IRenderEngine* renderEngine, float mass, Coord2d c);
		~RenderableBubble();

		void reset(float mass, float x, float y);
		void reset(float mass, Coord2d c);

		void render();
		void setFieldOfViewColor(Color color);

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

		void					updatePosition(float elapsedTime, agarai::Rectangle limits);
		void					updateSurface(float elapsedTime);

	};
}