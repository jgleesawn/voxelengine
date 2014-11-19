#ifndef TERRAIN_H
#define TERRAIN_H

#include <vector>
#include <utility>
#include "glmanager/glmanager.h"
#include "generator.h"
#include "object/renderable.h"

float plane( const float & x, const float & y, const float & z );

#define terrain_size 5

class Terrain {
public:
	GLmanager * glm;
	float chunk_size;
	glm::vec4 pos;
	Renderable * space[terrain_size][terrain_size][terrain_size];

	Terrain();
	~Terrain();

	void GenerateTerrain();
	std::vector<InstInfo> && getRenderVector();
};
//#undef terrain_size


#endif


