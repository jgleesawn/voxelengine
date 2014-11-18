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

struct gfxObj_t {
	int vao;
	int ind;
};

struct ObjGroup {
	GLuint VBO;
	GLuint IBO;
	GLuint VAO;
	size_t vbo_struct_size;
	size_t ibo_struct_size;
	std::vector<int> IBOStartingIndices;
	std::vector<int> VBOStartingIndices;
};

struct Inst {
	GLuint VBO;
	GLuint IBO;
	GLuint InstBO;
	GLuint VAO;
	unsigned int numIndicesPerInstance;
};

struct InstInfo {
	GLfloat position[3];
	GLfloat depthMask_in;
};

class GLmanager {
public:
	std::vector<ObjGroup *> gfxObjs;
	std::vector<Inst *> gfxInst;

	GLmanager() {}
	~GLmanager();
	std::vector<gfxObj_t> Load(const std::vector< ObjModel > & vobj );//std::vector<std::vector<glm::vec4 > * > * vbos_in, std::vector< std::vector<int> * > * ibos_in);
	std::vector<gfxObj_t> Load(const std::vector< std::string > & );
	int LoadInst(const ObjModel &, size_t = 100000);
	int LoadInst(const std::string &, size_t = 100000);

	void Update(gfxObj_t, std::vector<float>);

};

#endif
