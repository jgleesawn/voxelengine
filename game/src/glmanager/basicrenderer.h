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

class BasicRenderer {
protected:
	GLmanager * glm;

	GLuint theProgram;
	GLint * uvar;	//Uniform variables used because #120 doesn't support explicit locations
	int numUniforms;

	virtual void Initialize();
public:
	BasicRenderer(int numU = 6);
	~BasicRenderer() { glDeleteProgram(theProgram); delete uvar; }

	void setGLM(GLmanager *);

	void Use() { glUseProgram(theProgram); }
	void setCameraRotationPerspective(const glm::mat4 &, const glm::mat4 & );
	void setCameraPos(const glm::vec4 &);

	void Render(Renderable *, const glm::vec4 &);
	void Wireframe(Renderable *, const glm::vec4 &);

	void reshape(int, int);
};


#endif
