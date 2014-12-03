#include "instrenderer.h"

//Transpose = GL_TRUE because GLSL uses Column-Major where C++ typicall uses Row-Major
template<>
void InstRenderer<ObjModel>::RenderInst( const Inst & inst, const std::vector<InstInfo<ObjModel> > & vii, const glm::vec4 & llb, const float & res ) {
	glUseProgram(theProgram);
	glUniform4f(uvar[0], llb[0], llb[1], llb[2], 0.0f);
	glUniform1f(uvar[1], res);

	glUniform4f(uvar[5], 0.5f, 0.5f, 0.0f, 1.0f);

	glBindBuffer(GL_ARRAY_BUFFER, inst.InstBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(InstInfo<ObjModel>)*vii.size(), vii.data());
	glBindBuffer(GL_ARRAY_BUFFER,0);
	
	GLuint vao = inst.VAO;
	glBindVertexArray(vao);
	
	int count = vii.size();
	int cind = inst.numIndicesPerInstance;
	glDrawElementsInstanced( GL_TRIANGLES, cind, GL_UNSIGNED_INT, (void*)0, count );
	glBindVertexArray(0);
}

//USING LLB AS A UNIFORM COLOR AS A STOP-GAP MEASURE
//Coincides with note in terrain.h
//Transpose = GL_TRUE because GLSL uses Column-Major where C++ typicall uses Row-Major
template<>
void InstRenderer<ObjModel>::WireframeInst( const Inst & inst, const std::vector<InstInfo<ObjModel> > & vii, const glm::vec4 & llb, const float & res ) {
	glUseProgram(theProgram);
	glUniform4f(uvar[0], llb[0], llb[1], llb[2], 0.0f);
	glUniform1f(uvar[1], res);

	glUniform4f(uvar[5], llb[0], llb[1], llb[2], llb[3]); //0.0f, 1.0f, 1.0f, 1.0f);
	
	glBindBuffer(GL_ARRAY_BUFFER, inst.InstBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(InstInfo<ObjModel>)*vii.size(), vii.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	GLuint vao = inst.VAO;
	glBindVertexArray(vao);
	
	int count = vii.size();
	int cind = inst.numIndicesPerInstance;
	glDrawElementsInstanced( GL_LINE_STRIP, cind, GL_UNSIGNED_INT, (void*)0, count );
	glBindVertexArray(0);
}

//Should only work with T=ObjModel
//Move function to .cpp when you specialize
template<>
void InstRenderer<ObjModel>::DebugGrid() {
	glUseProgram(this->theProgram);
	glUniform1f(this->uvar[1], 1.0f);

	glUniform4f(this->uvar[5], 1.0f, 0.0f, 1.0f, 1.0f);

	glm::vec4 vbo[100][3][2];
	for( int i=0; i<100; i++ ) {
		for( int j=0; j<3; j++ ) {
			vbo[i][j][0] = glm::vec4(0.0f);
			vbo[i][j][1] = glm::vec4(0.0f);
			int prev = (j+2)%3;
			int next = (j+1)%3;
			vbo[i][j][0][j] = 1000.0f;
			vbo[i][j][1][j] = -1000.0f;
			vbo[i][j][0][prev] = 10*((i-50)/10);
			vbo[i][j][1][prev] = 10*((i-50)/10);
			vbo[i][j][0][next] = 10*((i-50)%10);
			vbo[i][j][1][next] = 10*((i-50)%10);
		}
	}
	int ibo[100][3][2];
	for( int i=0; i<100; i++ ) {
		for( int j=0; j<3; j++ ) {
			ibo[i][j][1] = j*2 + i*3;
			ibo[i][j][0] = 1 + j*2 + i*3;
		}
	}

	GLuint tmpVBO;
	glGenBuffers(1, &tmpVBO);
	glBindBuffer(GL_ARRAY_BUFFER, tmpVBO);
	glBufferData(GL_ARRAY_BUFFER, 600*sizeof(glm::vec4), vbo, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint tmpIBO;
	glGenBuffers(1, &tmpIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tmpIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 600*sizeof(int), ibo, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	InstInfo<ObjModel> ii;
	(*(glm::vec4 *)&ii.position) = glm::vec4(0.0f);
	ii.rotMat = glm::mat4(1.0f);
	ii.depthMask_in = 1.0f;
	GLuint tmpInstBO;
	glGenBuffers(1, &tmpInstBO);
	glBindBuffer(GL_ARRAY_BUFFER, tmpInstBO);
	glBufferData(GL_ARRAY_BUFFER, 1*sizeof(InstInfo<ObjModel>), &ii, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint tmpVAO;
	glGenVertexArrays(1, &tmpVAO);
	glBindVertexArray(tmpVAO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);
	glEnableVertexAttribArray(5);
	glBindBuffer( GL_ARRAY_BUFFER, tmpVBO );
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
	glBindBuffer( GL_ARRAY_BUFFER, tmpInstBO);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)(4*sizeof(GL_FLOAT)));
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)(8*sizeof(GL_FLOAT)));
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)(12*sizeof(GL_FLOAT)));
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)(16*sizeof(GL_FLOAT)));
	glVertexAttribDivisor(0,0);
	glVertexAttribDivisor(1,1);
	glVertexAttribDivisor(2,1);
	glVertexAttribDivisor(3,1);
	glVertexAttribDivisor(4,1);
	glVertexAttribDivisor(5,1);
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, tmpIBO);
	glBindVertexArray(0);
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer( GL_ARRAY_BUFFER, 0 );

	glFinish();
	glBindVertexArray(tmpVAO);
	glDrawElementsInstanced( GL_LINES, 600, GL_UNSIGNED_INT, (void*)0, 1 );
	glBindVertexArray(0);

	glDeleteBuffers(1, &tmpVBO);
	glDeleteBuffers(1, &tmpIBO);
	glDeleteBuffers(1, &tmpInstBO);
	glDeleteVertexArrays(1, &tmpVAO);

}


