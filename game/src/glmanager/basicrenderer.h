#ifndef BASICRENDERER_H
#define BASICRENDERER_H

//#include <GL/glew.h>
#include <string>
#include <vector>

#include <iostream>

#include "object/renderable.h"
#include "object/viewport.h"
#include "glprogrambase.h"
#include "glmanager.h"

//May have to change Render function to virtual.

template<typename T>
class BasicRenderer {
protected:
	GLmanager<T> * glm;

	GLuint theProgram;
	GLint * uvar;	//Uniform variables used because #120 doesn't support explicit locations
	int numUniforms;

public:
	virtual void Initialize();
	BasicRenderer(int numU = 6);
	~BasicRenderer() { glDeleteProgram(theProgram); delete uvar; }

	void setGLM(GLmanager<T> *);

	void Use() { glUseProgram(theProgram); }
	void setCameraRotationPerspective(const glm::mat4 &, const glm::mat4 & );
	void setCameraPos(const glm::vec4 &);

	void Render(Renderable *, const glm::vec4 &);
	void Wireframe(Renderable *, const glm::vec4 &);

	void reshape(int, int);
};


template<typename T>
void BasicRenderer<T>::Initialize() {
	std::vector<shaderName> shaderNames;
	shaderNames.push_back(shaderName(GL_VERTEX_SHADER, std::string("renderers/basic.v.shader")));
//	shaderNames.push_back(shaderName(GL_GEOMETRY_SHADER, std::string("renderers/basic.g.shader")));
	shaderNames.push_back(shaderName(GL_FRAGMENT_SHADER, std::string("renderers/basic.f.shader")));
	theProgram = GLProgramBase().InitializeProgram(shaderNames);

	std::cout << theProgram << std::endl;

	glUseProgram(theProgram);

	uvar[0] = glGetUniformLocation( theProgram, "objPosition");
	uvar[1] = glGetUniformLocation( theProgram, "objRotation");
	uvar[2] = glGetUniformLocation( theProgram, "viewOffset");
	uvar[3] = glGetUniformLocation( theProgram, "viewRotation");
	uvar[4] = glGetUniformLocation( theProgram, "Perspective");
	uvar[5] = glGetUniformLocation( theProgram, "uColor");

	for( int i=0; i<numUniforms; i++ )
		std::cout << uvar[i] << std::endl;
	glUseProgram(0);
}

//Need Init function so that the GL context is set up already;
//VE is created before GL context is set up.
template<typename T>
BasicRenderer<T>::BasicRenderer(int numU) : glm(NULL), numUniforms(numU) {
	uvar = new int[numU];
}

template<typename T>
void BasicRenderer<T>::setGLM(GLmanager<T> * glm_in) {
	glm = glm_in;
}

template<typename T>
void BasicRenderer<T>::setCameraRotationPerspective(const glm::mat4 & Rotation, const glm::mat4 & Perspective) {
	glm::mat4 rotpersp = Perspective*Rotation;
//Transposes matrix on glImport so multiplication must be reversed before transpose.
	glUniformMatrix4fv(uvar[3], 1, GL_TRUE, &rotpersp[0][0]);
	glUniformMatrix4fv(uvar[4], 1, GL_TRUE, &glm::mat4(1.0f)[0][0]);

//	glUniformMatrix4fv(uvar[3], 1, GL_TRUE, &Rotation[0][0]);
//	glUniformMatrix4fv(uvar[4], 1, GL_TRUE, &Perspective[0][0]);
}

template<typename T>
void BasicRenderer<T>::setCameraPos( const glm::vec4 & vpos ) {
	glUniform4f(uvar[2], vpos[0], vpos[1], vpos[2], 0.0f );
}

//Transpose = GL_TRUE because GLSL uses Column-Major where C++ typicall uses Row-Major
template<typename T>
void BasicRenderer<T>::Render( Renderable * objs, const glm::vec4 & op ) {
}

template<typename T>
void BasicRenderer<T>::Wireframe( Renderable * objs, const glm::vec4 & op ) {
}

template<typename T>
void BasicRenderer<T>::reshape (int w, int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}


#endif
