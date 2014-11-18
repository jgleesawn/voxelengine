#include "generator.h"

#define vbopos i+j*32+k*32*32

//order so x and z are contiguous, will lend for more contiguous data.
ObjModel * Generator::generate(const glm::vec4 & llb, const float & size, densityFunc df) {
	voxelpolymacro
	float densities[33*33*33];
	ObjModel * mesh = new ObjModel;

	mesh->vbo.reserve(32*32*32);
	mesh->vbo.resize(32*32*32);

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
			zoff = l&1;
			yoff = (l>>1)&1;
			xoff = (l>>2)&1;
			ind |= (densities[i + j*33 + k*33*33 + zoff + xoff*33 + yoff*33*33] > 0.0f ) << l;
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
					v.x = edge%2; 
					starting_vertex = edge;
					if( (starting_vertex-1)%4 )
						starting_vertex -= 3;
				} else if( (edge+1)%4 ) {
					v.y = (edge+1)%2;
					starting_vertex = edge;
					if( starting_vertex%4 )
						starting_vertex++;
				}
				dpos = i+j*33+k*33*33;
				end_offset = 33*(int)v.x + 33*33*(int)v.y + 1*(int)v.z;
				switch(starting_vertex) {
				case 0:
					d1 = densities[dpos];
					d2 = densities[dpos+end_offset];
					vboind = vbopos;
				break;
				case 1:
					d1 = densities[dpos+33*33];
					d2 = densities[dpos+33*33+end_offset];
					vboind = vbopos+32*32;
				break;
				case 2:
					d1 = densities[dpos+33+33*33];
					d2 = densities[dpos+33+33*33+end_offset];
					vboind = vbopos+32+32*32;
				break;
				case 3:
					d1 = densities[dpos+33];
					d2 = densities[dpos+33+end_offset];
					vboind = vbopos+32;
				break;
				case 4:
					d1 = densities[dpos+1];
					d2 = densities[dpos+1+end_offset];
					vboind = vbopos+1;
				break;
				case 5:
					d1 = densities[dpos+1+33*33];
					d2 = densities[dpos+1+33*33+end_offset];
					vboind = vbopos+1+32*32;
				break;
				case 6:
					d1 = densities[dpos+1+33+33*33];
					d2 = densities[dpos+1+33+33*33+end_offset];
					vboind = vbopos+1+32+32*32;
				break;
				case 7:
					d1 = densities[dpos+1+33];
					d2 = densities[dpos+1+33+end_offset];
					vboind = vbopos+1+32;
				break;
				}
				int z = vboind %32;
				int x = (vboind>>5) %32;
				int y = (vboind>>10) %32;	
				if( vboind > mesh->vbo.capacity() ) {
//					std::cout << vboind << " ind out of bounds " << mesh->vbo.capacity() << std::endl;
//					std::cout << vbopos << " pos out of bounds " << mesh->vbo.capacity() << std::endl;
//					for( int n=0; n<m; n++ ) 
//						mesh->ibo.pop_back();
//					break;
				}
				mesh->vbo[vboind] = ((float)(fabs(d1)/(fabs(d1)+fabs(d2)))*v)+glm::vec4((float)(j+x)*ss, (float)(k+y)*ss, (float)(i+z)*ss, 0.0f)+llb;
				mesh->ibo.push_back(vboind);
			}
		}
	}
	}
	}
	return mesh;
}

#undef vbopos

	
