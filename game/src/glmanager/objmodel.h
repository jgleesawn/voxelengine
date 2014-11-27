#ifndef OBJMODEL_H
#define OBJMODEL_H

#include <vector>
#include <map>
#include <glm/glm.hpp>

#include <btBulletDynamicsCommon.h>

struct ObjModel {
	std::vector<glm::vec4> vbo;
	std::vector<int> ibo;

//Compresses vbo so it does not have non-used vertices.
//Utilizes ibo to do this.
	void compress();
};



#endif
