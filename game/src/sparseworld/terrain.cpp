#include "terrain.h"

#include <glm/glm.hpp>
float plane( const float & x, const float & y, const float & z ) {
	return glm::sin(y/10.0f) + glm::sin(x/10.0f) + glm::sin(z/10.0f); //+ glm::cos(z);
//	return -y + glm::sin(z);
//	return x - y + glm::sin(z);
//	return glm::sin(z);
//	return glm::sin(x);
//	return glm::sin(y);
//	return glm::sin(x) + glm::sin(z);
	return glm::sin(y)-z;
//	return -x;
//	return -z;
//	return -x;
//	return -y;
//	return -((z<1 && z>-1)*2 - 1)*z;
//	return -z - y;
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
				pos_offset = pos;
				pos_offset.w = 1.0f;
//				pos_offset += glm::vec4((j-terrain_size/2)*chunk_size, (k-terrain_size/2)*chunk_size, (i-terrain_size/2)*chunk_size, 0.0f);
//				pos_offset += glm::vec4((i-terrain_size/2)*chunk_size, (j-terrain_size/2)*chunk_size, (k-terrain_size/2)*chunk_size, 0.0f);
				pos_offset += glm::vec4(i*chunk_size, j*chunk_size, k*chunk_size, 0.0f);
//				printv(pos_offset);
				if( !space[i][j][k] ) {
					ObjModel * obj_model = g.generate(pos_offset, chunk_size, plane);
					if( obj_model ) {
						space[i][j][k] = new Renderable(pos_offset, glm->LoadInst(*obj_model,1));
						delete obj_model;
					}
				}
			}
		}
	}
	std::cout << std::endl;
}

std::map< int, std::vector<InstInfo> > Terrain::getRenderMap() {
	int id, ind;
	std::map< int, std::vector<InstInfo> > renderInfo;
	InstInfo ii;
	for( int i=0; i<terrain_size; i++ ) {
		for( int j=0; j<terrain_size; j++ ) {
			for( int k=0; k<terrain_size; k++ ) {
				if( space[i][j][k] ) {
					id = space[i][j][k]->instance_id;
					if( glm->gfxInst[id] ) {
						ii.position[0] = alignment.x + pos.x + i * chunk_size/2.0f;
						ii.position[1] = alignment.y + pos.y + j * chunk_size/2.0f;
						ii.position[2] = alignment.z + pos.z + k * chunk_size/2.0f;
						ii.depthMask_in = 1.0f;
						renderInfo[id].push_back(ii);
					}
				}
			}
		}
	}
	return renderInfo;
}

void Terrain::MovePDim(int dim) {
	bool x,y,z;
	z = dim & 1;
	y = (dim >> 1) & 1;
	x = (dim >> 2) & 1;
//RemoveInst, Delete, set to null
	for( int i=0; i<1 + x*(terrain_size-1); i++ )
		for( int j=0; j<1 + y*(terrain_size-1); j++ )
			for( int k=0; k<1 + z*(terrain_size-1); k++ )
				if( space[i][j][k] ) {
					glm->RemoveInst(space[i][j][k]->instance_id);
					delete space[i][j][k];
					space[i][j][k] = NULL;
				}
//Move all one position
	for( int i=0; i<terrain_size-x; i++ )
		for( int j=0; j<terrain_size-y; j++ )
			for( int k=0; k<terrain_size-z; k++ ) {
				space[i][j][k] = space[i+x][j+y][k+z];
				space[i+x][j+y][k+z] = NULL;
			}
//Generate new chunk, set to null before-hand
	pos.x += chunk_size*(float)x;
	pos.y += chunk_size*(float)y;
	pos.z += chunk_size*(float)z;
	glm::vec4 pos_offset(0.0f);
	for( int i=x*(terrain_size-1); i<terrain_size; i++ )
		for( int j=y*(terrain_size-1); j<terrain_size; j++ )
			for( int k=z*(terrain_size-1); k<terrain_size; k++ ) {
				pos_offset = alignment + pos + glm::vec4(i*chunk_size, j*chunk_size, k*chunk_size, 0.0f);
				space[i][j][k] = NULL;
				ObjModel * obj_model = g.generate(pos_offset, chunk_size, plane);
				if( obj_model ) {
					space[i][j][k] = new Renderable(pos_offset, glm->LoadInst(*obj_model,1));
					delete obj_model;
				}
			}
}

void Terrain::MoveNDim(int dim) {
	bool x,y,z;
	z = dim & 1;
	y = (dim >> 1) & 1;
	x = (dim >> 2) & 1;
//RemoveInst, Delete, set to null
	for( int i=terrain_size-1; i>=0 + x*(terrain_size-1); i-- )
		for( int j=terrain_size-1; j>=0 + y*(terrain_size-1); j-- )
			for( int k=terrain_size-1; k>=0 + z*(terrain_size-1); k-- )
				if( space[i][j][k] ) {
					glm->RemoveInst(space[i][j][k]->instance_id);
					delete space[i][j][k];
					space[i][j][k] = NULL;
				}
//Move all one position
	for( int i=terrain_size-1 - x; i>=0; i-- )
		for( int j=terrain_size-1 - y; j>=0; j-- )
			for( int k=terrain_size-1 - z; k>=0; k-- ) {
				space[i+x][j+y][k+z] = space[i][j][k];
				space[i][j][k] = NULL;
			}
//Generate new chunk, set to null before-hand
	pos.x -= chunk_size*(float)x;
	pos.y -= chunk_size*(float)y;
	pos.z -= chunk_size*(float)z;
	glm::vec4 pos_offset(0.0f);
	for( int i=0; i<1+x*(terrain_size-1); i++ )
		for( int j=0; j<1+y*(terrain_size-1); j++ )
			for( int k=0; k<1+z*(terrain_size-1); k++ ) {
				pos_offset = alignment + pos + glm::vec4(i*chunk_size, j*chunk_size, k*chunk_size, 0.0f);
				space[i][j][k] = NULL;
				ObjModel * obj_model = g.generate(pos_offset, chunk_size, plane);
				if( obj_model ) {
					space[i][j][k] = new Renderable(pos_offset, glm->LoadInst(*obj_model,1));
					delete obj_model;
				}
			}
}

void Terrain::Center(glm::vec4 position, glm::vec4 direction) {
	glm::vec4 distal_point = position + direction * chunk_size * (terrain_size/2.0f);
	glm::vec4 max = glm::max(position, distal_point);
	glm::vec4 min = glm::min(position, distal_point);
	if( max.x > alignment.x + pos.x + chunk_size * terrain_size )
		MovePDim(4);
	if( min.x < alignment.x + pos.x + chunk_size)
		MoveNDim(4);
	if( max.y > alignment.y + pos.y + chunk_size * terrain_size )
		MovePDim(2);
	if( min.y < alignment.y + pos.y + chunk_size)
		MoveNDim(2);
	if( max.z > alignment.z + pos.z + chunk_size * terrain_size )
		MovePDim(1);
	if( min.z < alignment.z + pos.z + chunk_size)
		MoveNDim(1);
}





