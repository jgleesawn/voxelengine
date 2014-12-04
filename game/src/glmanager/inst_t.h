#ifndef INST_T_H
#define INST_T_H

#include <BulletCollision/CollisionShapes/btShapeHull.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include "objmodel.h"

template<typename T>
struct InstInfo { };

template<>
struct InstInfo<ObjModel> {
	GLfloat position[3];
	GLfloat depthMask_in;
	glm::mat4 rotMat;
};

template<typename T>
struct Inst_t {
	GLuint VBO;
	GLuint IBO;
	GLuint InstBO;
	GLuint VAO;
	unsigned int numIndicesPerInstance;

	Inst_t(const T & obj, size_t max_num_instances = 100000);
	Inst_t(btShapeHull & sh, size_t max_num_instances = 100000);
	~Inst_t();

	void setupVAO(const std::vector<size_t> & sizes);
	
};

template<typename T>
Inst_t<T>::~Inst_t() {
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IBO);
	glDeleteBuffers(1, &InstBO);
	glDeleteVertexArrays(1, &VAO);
}

#endif
