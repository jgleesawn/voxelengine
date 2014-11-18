#ifndef OBJMODEL_H
#define OBJMODEL_H

#include <vector>
#include <glm/glm.hpp>

struct ObjModel {
	std::vector<glm::vec4> vbo;
	std::vector<int> ibo;
};




#endif
