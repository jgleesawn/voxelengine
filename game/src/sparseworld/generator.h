#ifndef GENERATOR_H
#define GENERATOR_H

#include <iostream>

#include <glm/glm.hpp>
#include <cmath>
#include "glmanager/objmodel.h"
#include "polygons.h"

class Generator {
	typedef float(*densityFunc)(const float &, const float &, const float &);
public:
	ObjModel * generate(const glm::vec4 &, const float &, densityFunc);
};

#endif



