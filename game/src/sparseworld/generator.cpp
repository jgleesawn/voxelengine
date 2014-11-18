#include "generator.h"

#define vbopos i+j*32+k*32*32

//order so x and z are contiguous, will lend for more contiguous data.
ObjModel * Generator::generate(const glm::vec4 & llb, const float & size, densityFunc df) {
	voxelpolymacro
	float densities[33*33*33];
	ObjModel * mesh = new ObjModel;

	mesh->vbo.reserve(33*33*33);
	mesh->vbo.resize(33*33*33);

	int ibopos = 0;
	
	float ss = size/32.0f;
	for( int i=0; i<33; i++ ) {
	for( int j=0; j<33; j++ ) {
	for( int k=0; k<33; k++ ) {
		densities[i+j*33+k*33*33] = df(llb.x+j*ss, llb.y+k*ss, llb.z+i*ss);
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
	for( int i=0; i<32; i++ ) {
	for( int j=0; j<32; j++ ) {
	for( int k=0; k<32; k++ ) {
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
			ind |= (densities[i + j*33 + k*33*33 + zoff + xoff*33 + yoff*33*33] >= 0.0f ) << l;
		}
		if( ind == 0 || ind == 255 )
			continue;
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
				dpos = i+j*33+k*33*33;
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
				starting_offset += round(starting_voffset.x)*33;
				starting_offset += round(starting_voffset.y)*33*33;

				end_offset = round(v.z);
				end_offset += round(v.x)*33;
				end_offset += round(v.y)*33*33;

				d1 = densities[dpos + starting_offset];
				d2 = densities[dpos + starting_offset + end_offset];

				int vbo_offset = round(starting_voffset.z);
				vbo_offset += round(starting_voffset.x)*32;
				vbo_offset += round(starting_voffset.y)*32*32;

				vboind = vbopos + vbo_offset;

				int z = i + round(starting_voffset.z);
				int x = j*32 + round(starting_voffset.x)*32;
				int y = k*32*32 + round(starting_voffset.y)*32*32;
				
				if( vboind >= mesh->vbo.capacity() ) {
					printv(v);
					printv(starting_voffset);
					std::cout << x << " " << y << " " << z << std::endl;
					std::cout << vboind << " ind out of bounds " << mesh->vbo.capacity() << std::endl;
					std::cout << vbopos << " pos out of bounds " << mesh->vbo.capacity() << std::endl;
//					for( int n=0; n<m; n++ ) 
//						mesh->ibo.pop_back();
//					break;
				}
				float density_ratio = fabs(d1)/(fabs(d1)+fabs(d2));
			
				mesh->vbo[vboind] = starting_voffset*ss + density_ratio*v*ss + glm::vec4((float)j*ss, (float)k*ss, (float)i*ss, 0.0f) + llb;
//				mesh->vbo[vboind] = (density_ratio*v)+glm::vec4((float)(j+x)*ss, (float)(k+y)*ss, (float)(i+z)*ss, 0.0f)+llb;
//				mesh->vbo[vboind] = (density_ratio*v)+glm::vec4((float)(x)*ss, (float)(y)*ss, (float)(z)*ss, 0.0f)+llb;
				mesh->ibo.push_back(vboind);
			}
		}
	}
	}
	}
	return mesh;
}

#undef vbopos

	
