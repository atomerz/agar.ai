#pragma once

#include <cstdint>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "graphics/IRenderEngine.h"
#include "graphics/IRenderableObject.h"
#include "agar/Bubble.h"
#include "utils.h"


namespace agarai {

class RenderableBubble : public Bubble, public IRenderableObject {
  public:
	RenderableBubble(IRenderEngine* renderEngine, float mass, float x, float y, std::unique_ptr<Genome> genome=nullptr);
	RenderableBubble(IRenderEngine* renderEngine, float mass, Coord2d c, std::unique_ptr<Genome> genome=nullptr);
	~RenderableBubble();

	void reset(float mass, float x, float y, std::unique_ptr<Genome> genome=nullptr) override;
	void reset(float mass, Coord2d c, std::unique_ptr<Genome> genome=nullptr) override;
	void setBrain(std::unique_ptr<Brain>&& brain);

	void render();
	void setFieldOfViewColor(Color color);

  private:
	IRenderEngine*			renderEngine;
	//TODO: these can be static or placed somewhere to be created only once for all bubbles
	VertexBufferObject		lineVBO;
	VertexBufferObject		circleVBO;

	Color					color;
	Color					fovColor;

private:
	void					init(IRenderEngine* renderEngine);

    Color 					makeGeneticColor();

};

}