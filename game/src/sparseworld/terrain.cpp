#include "terrain.h"

#include <glm/glm.hpp>
float plane( const float & x, const float & y, const float & z ) {
	return glm::sin(y) + glm::sin(x) + glm::sin(z); //+ glm::cos(z);
	return -y;
}

Terrain::Terrain() {
	for( int i=0; i<terrain_size; i++ ) 
		for( int j=0; j<terrain_size; j++ ) 
			for( int k=0; k<terrain_size; k++ ) 
				space[i][j][k] = NULL;
}
Terrain::~Terrain() {
	for( int i=0; i<terrain_size; i++ ) 
		for( int j=0; j<terrain_size; j++ ) 
			for( int k=0; k<terrain_size; k++ ) 
				if(space[i][j][k])
					delete space[i][j][k];
}

void Terrain::GenerateTerrain() {
	Generator g;

	std::cout << "Generating terrain." << std::endl;
	glm::vec4 pos_offset(0.0f);
	for( int i=0; i<terrain_size; i++ ) {
		for( int j=0; j<terrain_size; j++ ) {
			for( int k=0; k<terrain_size; k++ ) {
				std::cerr << ".";
//				std::cout << k+j*terrain_size+i*terrain_size*terrain_size << " ";
				pos_offset = pos;
				pos_offset += glm::vec4((i-10)*chunk_size, (j-10)*chunk_size, -k*chunk_size, 0.0f);
				if( !space[i][j][k] )
					space[i][j][k] = new Renderable(pos_offset, glm->LoadInst(*g.generate(pos_offset, chunk_size, plane),1));
			}
		}
	}
	std::cout << std::endl;
}

std::vector<InstInfo> && Terrain::getRenderVector() {
	int id, ind;
	std::vector<InstInfo> rv;
	InstInfo ii;
	for( int i=0; i<terrain_size; i++ ) {
		for( int j=0; j<terrain_size; j++ ) {
			for( int k=0; k<terrain_size; k++ ) {
				id = space[i][j][k]->instance_id;
				ii.position[0] = space[i][j][k]->position.x;
				ii.position[1] = space[i][j][k]->position.y;
				ii.position[2] = space[i][j][k]->position.z;
				ii.depthMask_in = 1.0f;
				rv.push_back(ii);
			}
		}
	}
	return std::move(rv);
}
