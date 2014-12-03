#include "glmanager.h"

/*
template<>
struct InstInfo<ObjModel> {
	GLfloat position[3];
	GLfloat depthMask_in;
	glm::mat4 rotMat;
};
*/

template<>
int GLmanager<ObjModel>::LoadInst(const ObjModel & obj, size_t max_num_instances) {
	Inst * inst = new Inst;

	size_t vbo_struct_size = sizeof(*obj.vbo.data());
	size_t ibo_struct_size = sizeof(*obj.ibo.data());
	size_t inst_struct_size = sizeof(InstInfo<ObjModel>);
	size_t max_instances = max_num_instances;

	inst->numIndicesPerInstance = obj.ibo.size();
	
	glGenBuffers(1, &inst->VBO);
	glBindBuffer( GL_ARRAY_BUFFER, inst->VBO );
	glBufferData( GL_ARRAY_BUFFER, vbo_struct_size * obj.vbo.size(), obj.vbo.data(), GL_DYNAMIC_DRAW);

//	std::cout << "vbo size: " << vbo_struct_size * obj.vbo.size() << std::endl;
	
	glGenBuffers(1, &inst->IBO);
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, inst->IBO );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, ibo_struct_size * obj.ibo.size(), obj.ibo.data(), GL_DYNAMIC_DRAW);

//	std::cout << "ibo size: " << ibo_struct_size * obj.ibo.size() << std::endl;

	glGenBuffers(1, &inst->InstBO);
	glBindBuffer( GL_ARRAY_BUFFER, inst->InstBO );
	glBufferData( GL_ARRAY_BUFFER, inst_struct_size * max_instances, NULL, GL_DYNAMIC_DRAW);

//	std::cout << "instbo size: " << inst_struct_size * max_instances << std::endl;

	glGenVertexArrays(1, &inst->VAO);
	glBindVertexArray(inst->VAO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glBindBuffer( GL_ARRAY_BUFFER, inst->VBO );
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, vbo_struct_size, (void*) 0);
	glBindBuffer( GL_ARRAY_BUFFER, inst->InstBO );
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
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, inst->IBO );

	glBindVertexArray(0);
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	gfxInst.push_back(inst);

//	std::cout << inst->VBO << " " << inst->IBO << " " << inst->InstBO << " " << inst->VAO << std::endl;

	return gfxInst.size()-1;
}




