#include "inst_t.h"
#include "objmodel.h"


//Defined first so definition can be used in following constructors
template<>
void Inst_t<ObjModel>::setupVAO(const std::vector<size_t> & sizes) {
	size_t vbo_struct_size = sizes[0];
	size_t inst_struct_size = sizes[2];

	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glBindBuffer( GL_ARRAY_BUFFER, VBO );
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, vbo_struct_size, (void*) 0);
	glBindBuffer( GL_ARRAY_BUFFER, InstBO );
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, inst_struct_size, (void*) 0);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, inst_struct_size, (void*) (4*sizeof(GL_FLOAT)) );
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, inst_struct_size, (void*) (8*sizeof(GL_FLOAT)) );
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, inst_struct_size, (void*) (12*sizeof(GL_FLOAT)) );
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, inst_struct_size, (void*) (16*sizeof(GL_FLOAT)) );
	glVertexAttribDivisor(0,0);
	glVertexAttribDivisor(1,1);
	glVertexAttribDivisor(2,1);
	glVertexAttribDivisor(3,1);
	glVertexAttribDivisor(4,1);
	glVertexAttribDivisor(5,1);
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, IBO );

	glBindVertexArray(0);
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}


template<>
Inst_t<ObjModel>::Inst_t(const ObjModel & obj, size_t max_num_instances) {
	size_t vbo_struct_size = sizeof(*obj.vbo.data());
	size_t ibo_struct_size = sizeof(*obj.ibo.data());
	size_t inst_struct_size = sizeof(InstInfo<ObjModel>);
	size_t max_instances = max_num_instances;

	numIndicesPerInstance = obj.ibo.size();
	
	glGenBuffers(1, &VBO);
	glBindBuffer( GL_ARRAY_BUFFER, VBO );
	glBufferData( GL_ARRAY_BUFFER, vbo_struct_size * obj.vbo.size(), obj.vbo.data(), GL_DYNAMIC_DRAW);

//	std::cout << "vbo size: " << vbo_struct_size * obj.vbo.size() << std::endl;
	
	glGenBuffers(1, &IBO);
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, IBO );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, ibo_struct_size * obj.ibo.size(), obj.ibo.data(), GL_DYNAMIC_DRAW);

//	std::cout << "ibo size: " << ibo_struct_size * obj.ibo.size() << std::endl;

	glGenBuffers(1, &InstBO);
	glBindBuffer( GL_ARRAY_BUFFER, InstBO );
	glBufferData( GL_ARRAY_BUFFER, inst_struct_size * max_instances, NULL, GL_DYNAMIC_DRAW);

//	std::cout << "instbo size: " << inst_struct_size * max_instances << std::endl;

	glGenVertexArrays(1, &VAO);

	std::vector<size_t> sizes;
	sizes.push_back(vbo_struct_size);
	sizes.push_back(ibo_struct_size);
	sizes.push_back(inst_struct_size);

	setupVAO(sizes);
}

#include <iostream>
#include "util/glm_util.h"
template<>
Inst_t<ObjModel>::Inst_t(btShapeHull & sh, size_t max_num_instances) {
	sh.buildHull(0.0f);//Margin of 0.0f
	size_t vbo_struct_size = sizeof(*sh.getVertexPointer());
	size_t ibo_struct_size = sizeof(*sh.getIndexPointer());
	size_t inst_struct_size = sizeof(InstInfo<ObjModel>);
	size_t max_instances = max_num_instances;

	numIndicesPerInstance = sh.numIndices();

	for( int i=0; i<sh.numVertices(); i++ )
		printv(((glm::vec4*)sh.getVertexPointer())[i]);

	glGenBuffers(1, &VBO);
	glBindBuffer( GL_ARRAY_BUFFER, VBO );
	glBufferData( GL_ARRAY_BUFFER, vbo_struct_size * sh.numVertices(), sh.getVertexPointer(), GL_DYNAMIC_DRAW);

	glGenBuffers(1, &IBO);
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, IBO );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, ibo_struct_size * sh.numIndices(), sh.getIndexPointer(), GL_DYNAMIC_DRAW);

	glGenBuffers(1, &InstBO);
	glBindBuffer( GL_ARRAY_BUFFER, InstBO );
	glBufferData( GL_ARRAY_BUFFER, inst_struct_size * max_instances, NULL, GL_DYNAMIC_DRAW);

	glGenVertexArrays(1, &VAO);

	std::vector<size_t> sizes;
	sizes.push_back(vbo_struct_size);
	sizes.push_back(ibo_struct_size);
	sizes.push_back(inst_struct_size);

	setupVAO(sizes);
}



