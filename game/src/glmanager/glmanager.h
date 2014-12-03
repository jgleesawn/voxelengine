#ifndef GLMANAGER_H
#define GLMANAGER_H

#include <GL/glew.h>
//#include <GL/gl.h>
//#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>

#include <vector>
#include <string>
#include <utility>
#include <iostream>

#include "objmodel.h"
#include "objloader.h"

//Extend by making it a vector of vbos?
//Customized loading by templated Load function.
struct Inst {
	GLuint VBO;
	GLuint IBO;
	GLuint InstBO;
	GLuint VAO;
	unsigned int numIndicesPerInstance;
};

template <typename T>
struct InstInfo { };

template <>
struct InstInfo<ObjModel> {
	GLfloat position[3];
	GLfloat depthMask_in;
	glm::mat4 rotMat;
};

template <typename T>
class GLmanager {
public:
	std::vector<Inst *> gfxInst;

	GLmanager() {}
	~GLmanager();
	int LoadInst(const ObjModel &, size_t = 100000);
	int LoadInst(const std::string &, size_t = 100000);

	void RemoveInst(int);

};



template<typename T>
GLmanager<T>::~GLmanager() {
	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glBindBuffer( GL_ARRAY_BUFFER, 0);
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0);

	while( !gfxInst.empty() ) {
		if( gfxInst.back() ) {
			glDeleteBuffers(1, &gfxInst.back()->VBO);
			glDeleteBuffers(1, &gfxInst.back()->IBO);
			glDeleteBuffers(1, &gfxInst.back()->InstBO);
			glDeleteVertexArrays(1, &gfxInst.back()->VAO);
	
			delete gfxInst.back();
		}
		gfxInst.pop_back();
	}
}

template<typename T>
int GLmanager<T>::LoadInst(const std::string & fileName, size_t max_num_instances ) {
	return LoadInst(std::move(loadObjFile(fileName)), max_num_instances);
}

template<typename T>
void GLmanager<T>::RemoveInst(int instance_id) {
	if( gfxInst[instance_id] ) {
		glDeleteBuffers(1, &gfxInst[instance_id]->VBO);
		glDeleteBuffers(1, &gfxInst[instance_id]->IBO);
		glDeleteBuffers(1, &gfxInst[instance_id]->InstBO);
		glDeleteVertexArrays(1, &gfxInst[instance_id]->VAO);
		delete gfxInst[instance_id];
		gfxInst[instance_id] = NULL;
	}
}

#endif


