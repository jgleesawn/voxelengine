#include "generator.h"

#define NPD 32
#define DDD 33
#define vbopos i+j*NPD+k*NPD*NPD

#include <map>

//order so x and z are contiguous, will lend for more contiguous data.
ObjModel * Generator::generate(const glm::vec4 & llb, const float & size, densityFunc df) {
	std::map<int, int> counter;
	voxelpolymacro
	float densities[DDD*DDD*DDD];
	ObjModel * mesh = new ObjModel;

	mesh->vbo.reserve(DDD*DDD*DDD);
	mesh->vbo.resize(DDD*DDD*DDD);

	int ibopos = 0;
	
	float ss = size/(float)NPD;
	for( int i=0; i<DDD; i++ ) {
	for( int j=0; j<DDD; j++ ) {
	for( int k=0; k<DDD; k++ ) {
		densities[i+j*DDD+k*DDD*DDD] = df(llb.x+((float)j)*ss, llb.y+((float)k)*ss, llb.z+((float)i)*ss);
	}
	}
	}
//z is contiguous, then x, then y
//i                     j       k

	int zoff, yoff, xoff;
	int ind = 0;
	int edge, starting_vertex, dpos, end_offset;
	int vboind;
	float d1,d2;
	for( int i=0; i<NPD; i++ ) {
	for( int j=0; j<NPD; j++ ) {
	for( int k=0; k<NPD; k++ ) {
		ind = 0;
		//v7|v6|v5|v4|v3|v2|v1|v0
		for( int l=0; l<8; l++ ) {
//			zoff = l&1;
//			yoff = (l>>1)&1;
//			xoff = (l>>2)&1;
//Mapping indices as described in the nvidia tutorial, and as I've written in my notes.
			zoff = l>>2;
			yoff = (i&1) ^ ((i>>1)&1);
			xoff = (i>>1)&1;
			ind |= (densities[i + j*DDD + k*DDD*DDD + zoff + xoff*DDD + yoff*DDD*DDD] > 0.0f ) << l;
		}
//DEBUGGING
		counter[ind]++;

//		if( ind == 0 || ind == 255 )
//			continue;
		for( int l=0; l<voxelpoly.numPolys[ind]; l++ ) {
			for( int m=0; m<3; m++ ) {
				edge = voxelpoly.polyInd[ind*5+l].data[m];
				glm::vec4 v(0.0f);
				starting_vertex = -1;
				if (edge >= 8) {
					v.z = 1.0f; 
					starting_vertex = edge%4;
				} else if( edge % 4 ) {
					v.x = 1.0f; 
					starting_vertex = edge;
//					if( (starting_vertex-1)%4 )
//						starting_vertex -= 3;
				} else if( (edge+1)%4 ) {
					v.y = 1.0f;
					starting_vertex = edge;
//					if( starting_vertex%4 )
//						starting_vertex++;
				}

				dpos = i+j*DDD+k*DDD*DDD;
				glm::vec4 starting_voffset(0.0f);
				switch(starting_vertex) {
				case 0:
					starting_voffset = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
				break;
				case 1:
					starting_voffset = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);
				break;
				case 2:
					starting_voffset = glm::vec4(1.0f, 1.0f, 0.0f, 0.0f);
				break;
				case 3:
					starting_voffset = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
				break;
				case 4:
					starting_voffset = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
				break;
				case 5:
					starting_voffset = glm::vec4(0.0f, 1.0f, 1.0f, 0.0f);
				break;
				case 6:
					starting_voffset = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
				break;
				case 7:
					starting_voffset = glm::vec4(1.0f, 0.0f, 1.0f, 0.0f);
				break;
				}
				starting_voffset -= starting_voffset*v;

				int starting_offset = round(starting_voffset.z);
				starting_offset += round(starting_voffset.x)*DDD;
				starting_offset += round(starting_voffset.y)*DDD*DDD;

				end_offset = round(v.z);
				end_offset += round(v.x)*DDD;
				end_offset += round(v.y)*DDD*DDD;

				d1 = densities[dpos + starting_offset];
				d2 = densities[dpos + starting_offset + end_offset];

				int vbo_offset = round(starting_voffset.z);
				vbo_offset += round(starting_voffset.x)*DDD;
				vbo_offset += round(starting_voffset.y)*DDD*DDD;

//				vboind = vbopos + vbo_offset;
				vboind = dpos + vbo_offset;

				float density_ratio = fabs(d1)/(fabs(d1)+fabs(d2));
				mesh->vbo[vboind] = starting_voffset*ss + density_ratio*v*ss + glm::vec4((float)j*ss, (float)k*ss, (float)i*ss, 0.0f);// + llb;
//				mesh->vbo[vboind] = (density_ratio*v)+glm::vec4((float)(j+x)*ss, (float)(k+y)*ss, (float)(i+z)*ss, 0.0f)+llb;
//				mesh->vbo[vboind] = (density_ratio*v)+glm::vec4((float)(x)*ss, (float)(y)*ss, (float)(z)*ss, 0.0f)+llb;
				mesh->ibo.push_back(vboind);
			}
		}
	}
	}
	}
	std::map<int, int>::iterator it;
	for( it = counter.begin(); it!=counter.end(); it++ ) {
		if( it->first != 0 && it->first != 15 && it->first != 240 && it->first != 255 )
			std::cout << it->first << " " << it->second << std::endl;
	}
//	std::cout << "indices for " << llb.x << " " << llb.y << " " << llb.z << ": " << mesh->ibo.size() << std::endl;
	return mesh;
}

#undef vbopos
#undef NPD
#undef DDD 
	
