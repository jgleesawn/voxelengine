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

#include "inst_t.h"

template <typename T>
class GLmanager {
public:
	std::vector<Inst_t<T> *> gfxInst;

	GLmanager() {}
	~GLmanager();
	int LoadInst(Inst_t<T> *);
	int LoadInst(const T &, size_t = 100000);
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
			delete gfxInst.back();
		}
		gfxInst.pop_back();
	}
}

template<typename T>
int GLmanager<T>::LoadInst(Inst_t<T> * inst) {
	gfxInst.push_back(inst);
	return gfxInst.size()-1;
}

template<typename T>
int GLmanager<T>::LoadInst(const T & obj, size_t max_num_instances) {
	gfxInst.push_back(new Inst_t<T>(obj, max_num_instances));
	return gfxInst.size()-1;
}
template<typename T>
int GLmanager<T>::LoadInst(const std::string & fileName, size_t max_num_instances ) {
	gfxInst.push_back(new Inst_t<T>(std::move(loadObjFile(fileName)), max_num_instances));
	return gfxInst.size()-1;
}

template<typename T>
void GLmanager<T>::RemoveInst(int instance_id) {
	if( gfxInst[instance_id] ) {
		delete gfxInst[instance_id];
		gfxInst[instance_id] = NULL;
	}
}

#endif


