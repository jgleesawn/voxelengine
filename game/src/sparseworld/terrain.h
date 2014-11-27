#ifndef TERRAIN_H
#define TERRAIN_H

#include <map>
#include <vector>
#include <utility>
#include "glmanager/glmanager.h"
#include "generator.h"
#include "object/renderable.h"
#include "world.h"
#include "bullet/modelmesh.h"
#include "bullet/bulletbvh.h"

float plane( const float & x, const float & y, const float & z );

#define terrain_size 3

class World;

class Terrain {
public:
	GLmanager * glm;
	float chunk_size;
	glm::vec4 alignment;
	glm::vec4 pos;
	Renderable * space[terrain_size][terrain_size][terrain_size];
	int debug_instance_id;

	Generator g;	//Find a better way to handle this.  Generator/polygon class is large.
	World * w;

	Terrain(World *);
	~Terrain();

	void GenerateTerrain();
	std::map<int, std::vector<InstInfo> > getRenderMap();
	std::map<int, std::vector<InstInfo> > getDebugRenderMap(bool selector = false);
//selector is a stop-gap measure while colors are still passed as a uniform variable.
//Should remove this, add a color attribute to the renderer.
//On doing so, can use renderable check to set color for debug boxes.
//Will then use one call.

	void MovePDim(int);
	void MoveNDim(int);

	void Center(glm::vec4, glm::vec4);
};
//#undef terrain_size


#endif


