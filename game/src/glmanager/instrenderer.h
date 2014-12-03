#ifndef INSTRENDERER_H
#define INSTRENDERER_H

//#include <GL/glew.h>
#include <string>
#include <vector>

#include <iostream>

#include "object/renderable.h"
#include "object/viewport.h"
#include "glprogrambase.h"
#include "glmanager.h"
#include "basicrenderer.h"

//May have to change Render function to virtual.

template<typename T>
class InstRenderer : public BasicRenderer<T> {
public:
	virtual void Initialize();
	InstRenderer();
	~InstRenderer() { }//glDeleteProgram(theProgram); }

	void RenderInst(const Inst &, const std::vector<InstInfo<T> > &, const glm::vec4 &, const float &);
	void WireframeInst(const Inst &, const std::vector<InstInfo<T> > &, const glm::vec4 &, const float &);

	void DebugGrid();
};


template< typename T >
void InstRenderer<T>::Initialize() {
	std::vector<shaderName> shaderNames;
	shaderNames.push_back(shaderName(GL_VERTEX_SHADER, std::string("renderers/voxel.v.shader")));
//	shaderNames.push_back(shaderName(GL_GEOMETRY_SHADER, std::string("renderers/basic.g.shader")));
	shaderNames.push_back(shaderName(GL_FRAGMENT_SHADER, std::string("renderers/voxel.f.shader")));
	this->theProgram = GLProgramBase().InitializeProgram(shaderNames);

	std::cout << this->theProgram << std::endl;

	glUseProgram(this->theProgram);

//	GLint uvar[5];
	this->uvar[0] = glGetUniformLocation( this->theProgram, "lowerLeftBound");
	this->uvar[1] = glGetUniformLocation( this->theProgram, "resolution");
	this->uvar[2] = glGetUniformLocation( this->theProgram, "viewOffset");
	this->uvar[3] = glGetUniformLocation( this->theProgram, "viewRotation");
	this->uvar[4] = glGetUniformLocation( this->theProgram, "Perspective");
	this->uvar[5] = glGetUniformLocation( this->theProgram, "uColor");

	for( int i=0; i<this->numUniforms; i++ )
		std::cout << this->uvar[i] << std::endl;
	glUseProgram(0);
}

template<typename T>
InstRenderer<T>::InstRenderer() { 
//	std::cout << "inst" << std::endl;
	this->glm = NULL;
	this->uvar = new int[6];
	this->numUniforms = 6;
}


#endif
