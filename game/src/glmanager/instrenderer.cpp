#include "instrenderer.h"

void InstRenderer::Initialize() {
	std::vector<shaderName> shaderNames;
	shaderNames.push_back(shaderName(GL_VERTEX_SHADER, std::string("renderers/voxel.v.shader")));
//	shaderNames.push_back(shaderName(GL_GEOMETRY_SHADER, std::string("renderers/basic.g.shader")));
	shaderNames.push_back(shaderName(GL_FRAGMENT_SHADER, std::string("renderers/voxel.f.shader")));
	theProgram = GLProgramBase().InitializeProgram(shaderNames);

	std::cout << theProgram << std::endl;

	glUseProgram(theProgram);

//	GLint uvar[5];
	uvar[0] = glGetUniformLocation( theProgram, "lowerLeftBound");
	uvar[1] = glGetUniformLocation( theProgram, "resolution");
	uvar[2] = glGetUniformLocation( theProgram, "viewOffset");
	uvar[3] = glGetUniformLocation( theProgram, "viewRotation");
	uvar[4] = glGetUniformLocation( theProgram, "Perspective");
	uvar[5] = glGetUniformLocation( theProgram, "uColor");

	for( int i=0; i<numUniforms; i++ )
		std::cout << uvar[i] << std::endl;
	glUseProgram(0);
}

InstRenderer::InstRenderer() { 
//	std::cout << "inst" << std::endl;
	glm = NULL;
	uvar = new int[6];
	numUniforms = 6;
}


/*
void InstRenderer::setCameraRotationPerspective(const glm::mat4 & Rotation, const glm::mat4 & Perspective) {
	glm::mat4 rotpersp = Perspective*Rotation;
//Transposes matrix on glImport so multiplication must be reversed before transpose.
	glUniformMatrix4fv(uvar[3], 1, GL_TRUE, &rotpersp[0][0]);
	glUniformMatrix4fv(uvar[4], 1, GL_TRUE, &glm::mat4(1.0f)[0][0]);

//	glUniformMatrix4fv(uvar[3], 1, GL_TRUE, &Rotation[0][0]);
//	glUniformMatrix4fv(uvar[4], 1, GL_TRUE, &Perspective[0][0]);
}

void InstRenderer::setCameraPos( const glm::vec4 & vpos ) {
	glUniform4f(uvar[2], vpos[0], vpos[1], vpos[2], 0.0f );
}
*/

//Transpose = GL_TRUE because GLSL uses Column-Major where C++ typicall uses Row-Major
void InstRenderer::RenderInst( const Inst & inst, const std::vector<InstInfo> & vii, const glm::vec4 & llb, const float & res ) {
	glUseProgram(theProgram);
	glUniform4f(uvar[0], llb[0], llb[1], llb[2], 0.0f);
	glUniform1f(uvar[1], res);

//	for( int i=0; i<3; i++ )
//		std::cout << llb[i] << " ";
//	std::cout << std::endl;
//	std::cout << std::endl;
		
	glUniform4f(uvar[5], 0.5f, 0.5f, 0.0f, 1.0f);
	

	glBindBuffer(GL_ARRAY_BUFFER, inst.InstBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(InstInfo)*vii.size(), vii.data());
	glBindBuffer(GL_ARRAY_BUFFER,0);
	
	GLuint vao = inst.VAO;
	glBindVertexArray(vao);
	
	int count = vii.size();
	int cind = inst.numIndicesPerInstance;
//std::cout << inst.VBO << " " << inst.IBO << " " << inst.InstBO << " " << inst.VAO << std::endl;
//std::cout << cind << " " << count << std::endl;
//	glDrawElementsInstanced( GL_POINTS, cind, GL_UNSIGNED_INT, (void*)0, count );
	glDrawElementsInstanced( GL_TRIANGLES, cind, GL_UNSIGNED_INT, (void*)0, count );
//	glDrawElementsBaseVertex( GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)0, 0 );
	glBindVertexArray(0);
}

//Transpose = GL_TRUE because GLSL uses Column-Major where C++ typicall uses Row-Major
void InstRenderer::WireframeInst( const Inst & inst, const std::vector<InstInfo> & vii, const glm::vec4 & llb, const float & res ) {
	glUseProgram(theProgram);
	glUniform4f(uvar[0], llb[0], llb[1], llb[2], 0.0f);
	glUniform1f(uvar[1], res);
		
	glUniform4f(uvar[5], 0.0f, 1.0f, 1.0f, 1.0f);
	
	glBindBuffer(GL_ARRAY_BUFFER, inst.InstBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(InstInfo)*vii.size(), vii.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	GLuint vao = inst.VAO;
	glBindVertexArray(vao);
	
	int count = vii.size();
	int cind = inst.numIndicesPerInstance;
	glDrawElementsInstanced( GL_LINE_STRIP, cind, GL_UNSIGNED_INT, (void*)0, count );
	glBindVertexArray(0);
}

void InstRenderer::DebugGrid() {
	glUseProgram(theProgram);
	glUniform1f(uvar[1], 1.0f);

	glUniform4f(uvar[5], 0.0f, 0.0f, 1.0f, 1.0f);

	glm::vec4 vbo[100][3][2];
	for( int i=0; i<100; i++ ) {
		for( int j=0; j<3; j++ ) {
			vbo[i][j][0] = glm::vec4(0.0f);
			vbo[i][j][1] = glm::vec4(0.0f);
			int prev = (j+2)%3;
			int next = (j+1)%3;
			vbo[i][j][0][j] = 100.0f;
			vbo[i][j][1][j] = -100.0f;
			vbo[i][j][0][prev] = i/10;
			vbo[i][j][1][prev] = i/10;
			vbo[i][j][0][next] = i%10;
			vbo[i][j][1][next] = i%10;
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

	glm::vec4 pos(0.0f);
	pos.w = 1.0f;
	GLuint tmpInstBO;
	glGenBuffers(1, &tmpInstBO);
	glBindBuffer(GL_ARRAY_BUFFER, tmpInstBO);
	glBufferData(GL_ARRAY_BUFFER, 1*sizeof(glm::vec4), &pos, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	GLuint tmpVAO;
	glGenVertexArrays(1, &tmpVAO);
	glBindVertexArray(tmpVAO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glBindBuffer( GL_ARRAY_BUFFER, tmpVBO );
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
	glBindBuffer( GL_ARRAY_BUFFER, tmpInstBO);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
	glVertexAttribDivisor(0,0);
	glVertexAttribDivisor(1,1);
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
