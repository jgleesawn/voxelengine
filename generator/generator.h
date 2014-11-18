#ifndef GENERATOR_H
#define GENERATOR_H

#include <glm/glm.hpp>
#include <cmath>
#include "glmanager/objmodel.h"
#include "polygons.h"

struct Mesh {
	glm::vec4 vbo[32*32*32];
	int ibo[5*32*32*32];
	int indexCount;
};

class Generator {
	typedef float(*densityFunc)(const float &, const float &, const float &);
public:
	Mesh * generate(const glm::vec4 &, const float &, densityFunc);
};

#endif



