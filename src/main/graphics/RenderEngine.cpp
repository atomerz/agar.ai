#include "graphics/RenderEngine.h"
#include <iostream>
#include <sstream>
using namespace std;
using namespace agarai;
////////////////////////////////////////////////////////////////////////////////
const GLchar* defaultVertexShader =
{
	"attribute vec4 position;\n"\
	"uniform mat4 uPMatrix;\n"\
	"uniform mat4 uMVMatrix;\n"\

	"void main(void)\n"\
	"{\n"\
	"  gl_Position = uPMatrix * uMVMatrix * position;\n"\
	"}\n"
};
//------------------------------------------------------------------------------
const GLchar* defaultFragmentShader =
{
	"uniform vec4 uColor;\n"\

	"void main(void)\n"\
	"{\n"\
	"  gl_FragColor = uColor;\n"\
	"}\n"
};
////////////////////////////////////////////////////////////////////////////////
RenderEngine::RenderEngine()
{
	frameCount = 0;
	currentWindowSize.width = 800;
	currentWindowSize.height = 600;
}
//------------------------------------------------------------------------------
RenderEngine::~RenderEngine()
{
}
//------------------------------------------------------------------------------
void RenderEngine::init(int* argc, char** argv, Dimension2D windowSize)
{
	glutInit(argc, argv);

	currentWindowSize = windowSize;
	initWindow();
	
	GLenum result = glewInit();
	if (result != GLEW_OK )
	{
		cerr << "ERROR: " << glewGetErrorString(result) << endl;
		exit(EXIT_FAILURE);
	}

	cout << "OpenGL version: " << glGetString(GL_VERSION) << endl;

	initShaders();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
}
//------------------------------------------------------------------------------
void RenderEngine::addRenderableObject(IRenderableObject* object)
{
	renderList.push_back(object);
}
//------------------------------------------------------------------------------
void RenderEngine::onRender()
{
	frameCount++;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for(auto object : renderList)
		object->render();

	glutSwapBuffers();
	glutPostRedisplay();
}
//------------------------------------------------------------------------------
void RenderEngine::onWindowSizeChanged(int w, int h)
{
	currentWindowSize.width = w;
	currentWindowSize.height = h;

	glViewport(0, 0, w, h);
}
//------------------------------------------------------------------------------
void RenderEngine::onTimer(int value) {
	if (value != 0)	{
		stringstream sstr;
		sstr << windowTitlePrefix
			<< " - fps: " << frameCount * 4
			<< " @ " << currentWindowSize.width
			<< " x " << currentWindowSize.height;

		glutSetWindowTitle(sstr.str().c_str());
	}
  
	frameCount = 0;
}
//------------------------------------------------------------------------------
const Matrix4& RenderEngine::getProjectionMatrix() const
{
	return projectionMatrix;
}
//------------------------------------------------------------------------------
const Matrix4& RenderEngine::getModelViewMatrix() const
{
	return modelViewMatrix;
}
//------------------------------------------------------------------------------
void RenderEngine::setProjectionMatrix(const Matrix4& m)
{
	GLint pMatrixLocation = glGetUniformLocation(defaultShaderProgramId, "uPMatrix");
	glUniformMatrix4fv(pMatrixLocation, 1, GL_FALSE, m.get());
}
//------------------------------------------------------------------------------
void RenderEngine::setModelViewMatrix(const Matrix4& m)
{
	GLint mvMatrixLocation = glGetUniformLocation(defaultShaderProgramId, "uMVMatrix");
	glUniformMatrix4fv(mvMatrixLocation, 1, GL_FALSE, m.get());
}
//------------------------------------------------------------------------------
void RenderEngine::setColor(const Color& color)
{
	GLuint uColorLocation = glGetUniformLocation(defaultShaderProgramId, "uColor");
	glUniform4fv(uColorLocation, 1, color.getRGBA());
}
//------------------------------------------------------------------------------
void RenderEngine::initWindow()
{
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(
		GLUT_ACTION_ON_WINDOW_CLOSE,
		GLUT_ACTION_GLUTMAINLOOP_RETURNS);
  
	glutInitWindowSize(currentWindowSize.width, currentWindowSize.height);

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);

	int WindowHandle = glutCreateWindow(windowTitlePrefix.c_str());

	if(WindowHandle < 1)
	{
		cerr << "ERROR: Could not create a new rendering window.\n";
		exit(EXIT_FAILURE);
	}
}
//------------------------------------------------------------------------------
void RenderEngine::initShaders()
{
	createShader(defaultVertexShader, defaultFragmentShader,
		defaultShaderProgramId, defaultVertexShaderId, defaultFragmentShaderId);

	glUseProgram(defaultShaderProgramId);
}
//------------------------------------------------------------------------------
void RenderEngine::createShader(const char* vertexShader, const char* fragmentShader,
			GLuint& shaderProgramId, GLuint& vertexShaderId, GLuint& fragmentShaderId)
{
	vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderId, 1, &vertexShader, NULL);
	glCompileShader(vertexShaderId);

	fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderId, 1, &fragmentShader, NULL);
	glCompileShader(fragmentShaderId);

	shaderProgramId = glCreateProgram();
	glAttachShader(shaderProgramId, vertexShaderId);
	glAttachShader(shaderProgramId, fragmentShaderId);
	glLinkProgram(shaderProgramId);

	GLenum errorCheckValue = glGetError();
	if (errorCheckValue != GL_NO_ERROR)
	{
		cerr << "ERROR: Could not create the shaders: " << gluErrorString(errorCheckValue) << endl;
		exit(-1);
	}
}
//------------------------------------------------------------------------------
void RenderEngine::destroyShaders()
{
  glUseProgram(0);

  destroyShader(defaultShaderProgramId, defaultVertexShaderId, defaultFragmentShaderId);
}
//------------------------------------------------------------------------------
void RenderEngine::destroyShader(GLuint shaderProgramId, GLuint vertexShaderId, GLuint fragmentShaderId)
{
  glDetachShader(shaderProgramId, vertexShaderId);
  glDetachShader(shaderProgramId, fragmentShaderId);

  glDeleteShader(fragmentShaderId);
  glDeleteShader(vertexShaderId);

  glDeleteProgram(shaderProgramId);

  GLenum errorCheckValue = glGetError();
  if (errorCheckValue != GL_NO_ERROR)
  {
	  cerr << "ERROR: Could not destroy the shaders: " <<  gluErrorString(errorCheckValue) << endl;
	  exit(-1);
  }
}
//------------------------------------------------------------------------------
void RenderEngine::onCleanup()
{
	destroyShaders();
}
//------------------------------------------------------------------------------
VertexBufferObject RenderEngine::createVBO()
{
	GLuint vaoId, vboId;
	
	glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);

	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);

	GLuint positionLocation = glGetAttribLocation(defaultShaderProgramId, "position");
	glVertexAttribPointer(positionLocation, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(positionLocation);
	
	GLenum errorCheckValue = glGetError();
	if (errorCheckValue != GL_NO_ERROR)
	{
		stringstream msg;
		msg << "ERROR: Could not create a VBO: " << gluErrorString(errorCheckValue) << endl;
		throw new runtime_error(msg.str().c_str());
	}

	VertexBufferObject vbo;
	vbo.VaoId = vaoId;
	vbo.VboId = vboId;
	return vbo;
}
//------------------------------------------------------------------------------
void RenderEngine::destroyVBO(const VertexBufferObject& vbo)
{
	//glDisableVertexAttribArray(0);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &vbo.VboId);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vbo.VaoId);
	
	GLenum errorCheckValue = glGetError();
	if (errorCheckValue != GL_NO_ERROR)
	{
		stringstream msg;
		msg << "ERROR: Could not destroy the VBO: " << gluErrorString(errorCheckValue) << endl;
		throw new runtime_error(msg.str().c_str());
	}
}
//------------------------------------------------------------------------------
void RenderEngine::render(const VertexBufferObject& vbo, RenderMode mode)
{
	glBindVertexArray(vbo.VaoId);
	glBindBuffer(GL_ARRAY_BUFFER, vbo.VboId);

	glDrawArrays(mode, 0, vbo.getVertexCount());
}