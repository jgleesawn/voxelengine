#include "glmanager.h"

GLmanager::~GLmanager() {
	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glBindBuffer( GL_ARRAY_BUFFER, 0);
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0);

	while( !gfxObjs.empty() ) {
		glDeleteBuffers(1, &gfxObjs.back()->VBO);
		glDeleteBuffers(1, &gfxObjs.back()->IBO);
		glDeleteVertexArrays(1, &gfxObjs.back()->VAO);

		delete gfxObjs.back();
		gfxObjs.pop_back();
	}
	while( !gfxInst.empty() ) {
		if( gfxInst.back() ) {
			glDeleteBuffers(1, &gfxInst.back()->VBO);
			glDeleteBuffers(1, &gfxInst.back()->IBO);
			glDeleteBuffers(1, &gfxInst.back()->InstBO);
			glDeleteVertexArrays(1, &gfxInst.back()->VAO);
	
			delete gfxInst.back();
		}
		gfxInst.pop_back();
	}
}

//vbos are updated	(DYNAMIC)
//ibos are not		(STATIC)
std::vector<gfxObj_t> GLmanager::Load( const std::vector< ObjModel > & vobj ) {
	std::vector<gfxObj_t> out;
	if( vobj.empty() )
		return out;

	int vbo_size = 0;
	int ibo_size = 0;
	for( int i=0; i<vobj.size(); i++ ) {
		vbo_size += vobj[i].vbo.size();
		ibo_size += vobj[i].ibo.size();
	}

	ObjGroup * og = new ObjGroup;

	og->vbo_struct_size = sizeof(*vobj[0].vbo.data());
	og->ibo_struct_size = sizeof(*vobj[0].ibo.data());
	
	glGenBuffers(1, &og->VBO);
	glBindBuffer( GL_ARRAY_BUFFER, og->VBO);
	glBufferData( GL_ARRAY_BUFFER, vbo_size * og->vbo_struct_size, NULL, GL_DYNAMIC_DRAW);
		
	glGenBuffers(1, &og->IBO);
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, og->IBO);
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, ibo_size * og->ibo_struct_size, NULL, GL_STATIC_DRAW);

	glGenVertexArrays(1, &og->VAO);
	glBindVertexArray(og->VAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, og->vbo_struct_size, (void*) 0);
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, og->IBO);
	glBindVertexArray(0);
	glDisableVertexAttribArray(0);

	int vbo_offset, ibo_offset;
	vbo_offset = ibo_offset = 0;	

	for( int i=0; i<vobj.size(); i++ ) {
		gfxObj_t go = {(int)gfxObjs.size(), i};
		out.push_back(go);

		og->VBOStartingIndices.push_back(vbo_offset);
		og->IBOStartingIndices.push_back(ibo_offset);
		glBufferSubData( GL_ARRAY_BUFFER, vbo_offset*og->vbo_struct_size, vobj[i].vbo.size()*og->vbo_struct_size, vobj[i].vbo.data() );
		vbo_offset += vobj[i].vbo.size();
		glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, ibo_offset*og->ibo_struct_size, vobj[i].ibo.size()*og->ibo_struct_size, vobj[i].ibo.data() );
		ibo_offset += vobj[i].ibo.size();
	}
//	std::cout << vbo_offset << std::endl << ibo_offset << std::endl;
	og->VBOStartingIndices.push_back(vbo_offset);
	og->IBOStartingIndices.push_back(ibo_offset);

	gfxObjs.push_back(og);

	glBindBuffer( GL_ARRAY_BUFFER, 0);
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0);

	return out;
}

std::vector<gfxObj_t> GLmanager::Load(const std::vector< std::string > & fileNames ) {
	std::vector<ObjModel> vobj;
	for( int i=0; i<fileNames.size(); i++ )
		vobj.push_back(std::move(loadObjFile(fileNames[i])));
	return Load(vobj);
}

int GLmanager::LoadInst(const ObjModel & obj, size_t max_num_instances) {
	Inst * inst = new Inst;

	size_t vbo_struct_size = sizeof(*obj.vbo.data());
	size_t ibo_struct_size = sizeof(*obj.ibo.data());
	size_t inst_struct_size = sizeof(InstInfo);
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
//	glEnableVertexAttribArray(2);
	glBindBuffer( GL_ARRAY_BUFFER, inst->VBO );
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, vbo_struct_size, (void*) 0);
	glBindBuffer( GL_ARRAY_BUFFER, inst->InstBO );
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, inst_struct_size, (void*) 0);
	glVertexAttribDivisor(0,0);
	glVertexAttribDivisor(1,1);
//	glVertexAttribDivisor(2,1);
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, inst->IBO );

	glBindVertexArray(0);
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	gfxInst.push_back(inst);

//	std::cout << inst->VBO << " " << inst->IBO << " " << inst->InstBO << " " << inst->VAO << std::endl;

	return gfxInst.size()-1;
}

int GLmanager::LoadInst(const std::string & fileName, size_t max_num_instances ) {
	return LoadInst(std::move(loadObjFile(fileName)), max_num_instances);
}

void GLmanager::RemoveInst(int instance_id) {
	if( gfxInst[instance_id] ) {
		glDeleteBuffers(1, &gfxInst[instance_id]->VBO);
		glDeleteBuffers(1, &gfxInst[instance_id]->IBO);
		glDeleteBuffers(1, &gfxInst[instance_id]->InstBO);
		glDeleteVertexArrays(1, &gfxInst[instance_id]->VAO);
		delete gfxInst[instance_id];
		gfxInst[instance_id] = NULL;
	}
}




