#ifndef TERRAIN_H
#define TERRAIN_H

#include <map>
#include <vector>
#include <utility>
#include "glmanager/glmanager.h"
#include "generator.h"
#include "object/renderable.h"

float plane( const float & x, const float & y, const float & z );

#define terrain_size 4

class Terrain {
public:
	GLmanager * glm;
	float chunk_size;
	glm::vec4 alignment;
	glm::vec4 pos;
	Renderable * space[terrain_size][terrain_size][terrain_size];

	Generator g;	//Find a better way to handle this.  Generator/polygon class is large.

	Terrain();
	~Terrain();

	void GenerateTerrain();
	std::map<int, std::vector<InstInfo> > getRenderMap();

	void MovePDim(int);
	void MoveNDim(int);

	void Center(glm::vec4, glm::vec4);
};
//#undef terrain_size


#endif


