#include "terrain.h"

#include <glm/glm.hpp>
noise::module::Perlin p_module;
noise::module::RidgedMulti rm_module;
float plane( const float & x, const float & y, const float & z ) {
	return 20 + y;
	float density;
	rm_module.SetFrequency(1/100.0f);
//	p_module.SetFrequency(1/10.0f);
//	p_module.SetPersistence(0.1);
//	density = 10.0f*p_module.GetValue(x, y, z) + y + 20;
	density = 20.0f*rm_module.GetValue(x, 0, z) + y + 20;
//	std::cerr << density << " ";
	return density;
	glm::vec3 v(x,y,z);
	v = 1000.0f*glm::sin(v/1000.0f) + 200.0f*glm::cos(v/500.0f) + 100.0f*glm::sin(v/200.0f) + 50.0f*glm::sin(v/75.0f) + 25.0f*glm::cos(v/37.5f) + 12.5f*glm::sin(v/20.0f) + 6.75f*glm::cos(v/7.438f) + 2.1823f*glm::sin(v);
	return glm::dot(glm::vec3(1.0f, 1.0f, 1.0f), v);

//	return y;
//	return 20 + y;

//	return glm::sin((x*x+y*y+z*z + 4)/30.0f);

//	return glm::sin(y/10.0f) + glm::sin(x/10.0f) + glm::sin(z/10.0f); //+ glm::cos(z);

//	return -y + glm::sin(z);
//	return x - y + glm::sin(z);
//	return glm::sin(z);
//	return glm::sin(x);
//	return glm::sin(y);
	return glm::sin(x) + glm::sin(z) - y;
	return glm::sin(y)-z;
//	return -x;
//	return -z;
//	return -x;
//	return -y;
//	return -((z<1 && z>-1)*2 - 1)*z;
//	return -z - y;
}

Terrain::Terrain(World * w_in) {
	w = w_in;
	for( int i=0; i<terrain_size; i++ ) 
		for( int j=0; j<terrain_size; j++ ) 
			for( int k=0; k<terrain_size; k++ ) 
				space[i][j][k] = NULL;
}
Terrain::~Terrain() {
	for( int i=0; i<terrain_size; i++ ) 
		for( int j=0; j<terrain_size; j++ ) 
			for( int k=0; k<terrain_size; k++ ) 
				if(space[i][j][k]) {
					w->removeObject(space[i][j][k]->index);
				}
}

void Terrain::setValues(GLmanager * glm_in) {
	glm = glm_in;
	debug_instance_id = 0;
	chunk_size = 10*2*3.1415f;
	alignment = glm::vec4(0.0f);
	alignment.x -= 2*3.1415f;
	alignment.y -= 2*3.1415f;
	alignment.z -= 2*3.1415f;
	pos = glm::vec4(0.0f);
}

void Terrain::GenerateTerrain() {
	Generator g;

	std::cout << "Generating terrain." << std::endl;
	glm::vec4 pos_offset(0.0f);
	for( int i=0; i<terrain_size; i++ ) {
		for( int j=0; j<terrain_size; j++ ) {
			for( int k=0; k<terrain_size; k++ ) {
				std::cerr << ".";
				pos_offset = alignment + pos;
				pos_offset.w = 1.0f;
//				pos_offset += glm::vec4((j-terrain_size/2)*chunk_size, (k-terrain_size/2)*chunk_size, (i-terrain_size/2)*chunk_size, 0.0f);
//				pos_offset += glm::vec4((i-terrain_size/2)*chunk_size, (j-terrain_size/2)*chunk_size, (k-terrain_size/2)*chunk_size, 0.0f);
				pos_offset += glm::vec4((float)i*chunk_size, (float)j*chunk_size, (float)k*chunk_size, 0.0f);
//				printv(pos_offset);
				if( !space[i][j][k] ) {
					ObjModel * obj_model = g.generate(pos_offset, chunk_size, plane);
					if( obj_model ) {
//reduced spacing for creation of triangle mesh shape from object model
ModelMesh mm(obj_model);
BvhTriangleMeshShapeContainer* mesh_shape = new BvhTriangleMeshShapeContainer(mm);
mm.release();
glm::quat q;

						space[i][j][k] = new Renderable(pos_offset, glm->LoadInst(*obj_model,1),q, 0, 0, 0, mesh_shape);
						space[i][j][k]->rigidBody->setRestitution(100.0f);
						w->addObject(space[i][j][k], glm::vec4(0.0f));
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
						ii.position[0] = alignment.x + pos.x + i * chunk_size;
						ii.position[1] = alignment.y + pos.y + j * chunk_size;
						ii.position[2] = alignment.z + pos.z + k * chunk_size;
//Bullet requires non-offset vertices.  So all vertices have been moved to true coordinates.
//Work on World offset and loading based on precision bounds.
//Will seem to be true coordinates to Bullet but will be relative coordinates to the entire world.
//Size just needs to be larger than small chunking I was using.
//						ii.position[0] = 0;
//						ii.position[1] = -8.0;
//						ii.position[1] = 0;
//						ii.position[2] = 0;

						ii.depthMask_in = 1.0f;
						ii.rotMat = glm::transpose(glm::mat4(1.0f));
						renderInfo[id].push_back(ii);
					}
				}
			}
		}
	}
	return renderInfo;
}

std::map<int, std::vector<InstInfo> > Terrain::getDebugRenderMap(bool selector) {
	int ind;
	std::map< int, std::vector<InstInfo> > renderInfo;
	InstInfo ii;
	for( int i=0; i<terrain_size; i++ ) {
		for( int j=0; j<terrain_size; j++ ) {
			for( int k=0; k<terrain_size; k++ ) {
				if( (bool)space[i][j][k] ^ selector ) {
					if( glm->gfxInst[debug_instance_id] ) {
//chunk_size was divided by 2.0f
						ii.position[0] = alignment.x + pos.x + i * chunk_size + chunk_size/2.0f;
						ii.position[1] = alignment.y + pos.y + j * chunk_size + chunk_size/2.0f;
						ii.position[2] = alignment.z + pos.z + k * chunk_size + chunk_size/2.0f;
						ii.depthMask_in = chunk_size/2.0f;
						ii.rotMat = glm::transpose(glm::mat4(1.0f));
						renderInfo[debug_instance_id].push_back(ii);
					}
				}
			}
		}
	}
	return renderInfo;
}

void Terrain::MovePDim(int dim) {
	bool x,y,z;
	x = y = z = 0;
	z = dim & 1;
	y = (dim >> 1) & 1;
	x = (dim >> 2) & 1;
//RemoveInst, Delete, set to null
	for( int i=0; i<1 + ((x+1)%2)*(terrain_size-1); i++ )
		for( int j=0; j<1 + ((y+1)%2)*(terrain_size-1); j++ )
			for( int k=0; k<1 + ((z+1)%2)*(terrain_size-1); k++ )
				if( space[i][j][k] ) {
					glm->RemoveInst(space[i][j][k]->instance_id);
					w->removeObject(space[i][j][k]->index);
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
//reduced spacing for creation of triangle mesh shape from object model
ModelMesh mm(obj_model);
BvhTriangleMeshShapeContainer* mesh_shape = new BvhTriangleMeshShapeContainer(mm);
mm.release();
glm::quat q;

					space[i][j][k] = new Renderable(pos_offset, glm->LoadInst(*obj_model,1),q, 0, 0, 0, mesh_shape);
					space[i][j][k]->rigidBody->setRestitution(100.0f);
					w->addObject(space[i][j][k], glm::vec4(0.0f));
				}
			}
}

void Terrain::MoveNDim(int dim) {
	bool x,y,z;
	x = y = z = 0;
	z = dim & 1;
	y = (dim >> 1) & 1;
	x = (dim >> 2) & 1;
//RemoveInst, Delete, set to null
	for( int i=terrain_size-1; i>=0 + x*(terrain_size-1); i-- )
		for( int j=terrain_size-1; j>=0 + y*(terrain_size-1); j-- )
			for( int k=terrain_size-1; k>=0 + z*(terrain_size-1); k-- )
				if( space[i][j][k] ) {
					glm->RemoveInst(space[i][j][k]->instance_id);
					w->removeObject(space[i][j][k]->index);
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
	for( int i=0; i<1+((x+1)%2)*(terrain_size-1); i++ )
		for( int j=0; j<1+((y+1)%2)*(terrain_size-1); j++ )
			for( int k=0; k<1+((z+1)%2)*(terrain_size-1); k++ ) {
				pos_offset = alignment + pos + glm::vec4(i*chunk_size, j*chunk_size, k*chunk_size, 0.0f);
				space[i][j][k] = NULL;
				ObjModel * obj_model = g.generate(pos_offset, chunk_size, plane);
				if( obj_model ) {
//reduced spacing for creation of triangle mesh shape from object model
ModelMesh mm(obj_model);
BvhTriangleMeshShapeContainer* mesh_shape = new BvhTriangleMeshShapeContainer(mm);
mm.release();
glm::quat q;

					space[i][j][k] = new Renderable(pos_offset, glm->LoadInst(*obj_model,1),q, 0, 0, 0, mesh_shape);
					space[i][j][k]->rigidBody->setRestitution(100.0f);
					w->addObject(space[i][j][k], glm::vec4(0.0f));
				}
			}
}

void Terrain::Center(glm::vec4 position, glm::vec4 direction) {
/*
	if( position.x > alignment.x + pos.x + chunk_size*(terrain_size/2.0f + 1.0f) )
		MovePDim(4);
	if( position.x < alignment.x + pos.x + chunk_size*(terrain_size/2.0f - 1.0f) )
		MoveNDim(4);

	if( position.y > alignment.y + pos.y + chunk_size*(terrain_size/2.0f + 1.0f) )
		MovePDim(2);
	if( position.y < alignment.y + pos.y + chunk_size*(terrain_size/2.0f - 1.0f) )
		MoveNDim(2);

	if( position.z > alignment.z + pos.z + chunk_size*(terrain_size/2.0f + 1.0f) )
		MovePDim(1);
	if( position.z < alignment.z + pos.z + chunk_size*(terrain_size/2.0f - 1.0f) )
		MoveNDim(1);
*/

	glm::vec4 distal_point = position + direction * chunk_size * (terrain_size/2.0f);
	if( distal_point.x > alignment.x + pos.x + chunk_size * (terrain_size + 1.0f))
		MovePDim(4);
	if( distal_point.x < alignment.x + pos.x - chunk_size )
		MoveNDim(4);
	if( distal_point.y > alignment.y + pos.y + chunk_size * (terrain_size + 1.0f))
		MovePDim(2);
	if( distal_point.y < alignment.y + pos.y - chunk_size )
		MoveNDim(2);
	if( distal_point.z > alignment.z + pos.z + chunk_size * (terrain_size + 1.0f))
		MovePDim(1);
	if( distal_point.z < alignment.z + pos.z - chunk_size )
		MoveNDim(1);

	if( position.x < alignment.x + pos.x + chunk_size )
		MoveNDim(4);
	if( position.x > alignment.x + pos.x + chunk_size*(terrain_size - 1.0f) )
		MovePDim(4);
	if( position.y < alignment.y + pos.y + chunk_size )
		MoveNDim(2);
	if( position.y > alignment.y + pos.y + chunk_size*(terrain_size - 1.0f) )
		MovePDim(2);
	if( position.z < alignment.z + pos.z + chunk_size )
		MoveNDim(1);
	if( position.z > alignment.z + pos.z + chunk_size*(terrain_size - 1.0f) )
		MovePDim(1);

}





