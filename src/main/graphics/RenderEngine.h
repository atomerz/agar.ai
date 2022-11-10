#pragma once
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "graphics/IRenderEngine.h"
#include "graphics/IRenderableObject.h"
#include <string>
#include <vector>
////////////////////////////////////////////////////////////////////////////////
namespace agarai
{
	class RenderEngine : public IRenderEngine
	{
	public:
		RenderEngine();
		~RenderEngine();
	
		void init(int* argc, char** argv, Dimension2D windowSize);
		void addRenderableObject(IRenderableObject* object);
		void onRender();
		void onWindowSizeChanged(int w, int h);
		void onTimer(int value);
		void onCleanup();

		VertexBufferObject createVBO();
		void destroyVBO(const VertexBufferObject& vbo);
		void render(const VertexBufferObject& vb, RenderMode mode);
	
		const Matrix4& getProjectionMatrix() const;
		const Matrix4& getModelViewMatrix() const;

		void setProjectionMatrix(const Matrix4& m);
		void setModelViewMatrix(const Matrix4& m);
		void setColor(const Color& color);
		
	private:

		void initWindow();
		void initShaders();
		void createShader(const char* vertexShader, const char* fragmentShader,
			GLuint& shaderProgramId, GLuint& vertexShaderId, GLuint& fragmentShaderId);
		
		void destroyShaders();
		void destroyShader(GLuint shaderProgramId, GLuint vertexShaderId, GLuint fragmentShaderId);

	private:
		std::string windowTitlePrefix;
		Dimension2D currentWindowSize;

		Matrix4 projectionMatrix;
		Matrix4 modelViewMatrix;

		unsigned int frameCount;

		GLuint defaultShaderProgramId;
		GLuint defaultVertexShaderId;
		GLuint defaultFragmentShaderId;

		std::vector<IRenderableObject*> renderList;

	};
}