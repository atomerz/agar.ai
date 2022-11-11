#pragma once
#include "Matrices.h"
#include "utils.h"
////////////////////////////////////////////////////////////////////////////////
namespace agarai
{
	/**
	* Represents an RGBA color.
	* RGBA values must be in range [0.0f, 1.0f].
	*/
	class Color
	{
	public:
		Color(float r=0, float g=0, float b=0, float a=1)
		{
			rgba[0] = r;
			rgba[1] = g;
			rgba[2] = b;
			rgba[3] = a;
		}

		const float* getRGBA() const
		{
			return rgba;
		}

		static Color randomize()
		{
			return Color(randf(), randf(), randf());
		}

	private:
		float rgba[4];
	};
	////////////////////////////////////////////////////////////////////////////////
	class Vertex
	{
	public:
		Vertex(float x=0, float y=0, float z=0, float w=1) 
		{
			setXYZW(x, y, z, w);
		}

		void setXYZW(float x, float y, float z, float w=1)
		{
			xyzw[0] = x;
			xyzw[1] = y;
			xyzw[2] = z;
			xyzw[3] = w;
		}

		const float* getXYZW() const
		{
			return xyzw;
		}

	private:
		float xyzw[4];
	};
	////////////////////////////////////////////////////////////////////////////////
	class VertexBufferObject {
	public:
		GLuint VaoId;
		GLuint VboId;

	public:
		VertexBufferObject()
		{
			vertexCount = 0;
		}

		enum Usage {
			VBO_USAGE_STATIC_DRAW = GL_STATIC_DRAW,
			VBO_USAGE_DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
		};
		void setBufferData(const Vertex* vertices, size_t vertexCount, Usage usage)
		{
			this->vertexCount = vertexCount;

			std::vector<float> vertexData;
			for(size_t i=0; i<vertexCount; ++i)
				for(size_t j=0; j<4; ++j)
					vertexData.push_back(vertices[i].getXYZW()[j]);

			glBindVertexArray(VaoId);
			glBindBuffer(GL_ARRAY_BUFFER, VboId);
			glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertexData.data(), GL_STATIC_DRAW);
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
		}

		size_t getVertexCount() const
		{
			return vertexCount;
		}


	private:
		size_t vertexCount;

	};
	////////////////////////////////////////////////////////////////////////////////
	class IRenderEngine
	{
	public:
		virtual VertexBufferObject createVBO() = 0;
		virtual void			destroyVBO(const VertexBufferObject& vbo) = 0;

		enum RenderMode
		{
			RENDER_MODE_POINTS = GL_POINTS,
			RENDER_MODE_LINES = GL_LINES,
			RENDER_MODE_LINE_LOOP = GL_LINE_LOOP,
			RENDER_MODE_POLYGON = GL_POLYGON,
		};
		virtual void render(const VertexBufferObject& vb, RenderMode mode) = 0;

		virtual const Matrix4&	getModelViewMatrix() const = 0;
		virtual void			setModelViewMatrix(const Matrix4& m) = 0;
		virtual const Matrix4&	getProjectionMatrix() const = 0;
		virtual void			setProjectionMatrix(const Matrix4& m) = 0;

		virtual void			setColor(const Color& color) = 0;
	};
}