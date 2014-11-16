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
		
	glUniform4f(uvar[5], 0.5f, 0.5f, 0.0f, 1.0f);
	

	glBindBuffer(GL_ARRAY_BUFFER, inst.InstBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(InstInfo)*vii.size(), vii.data());
	unsigned int * uptr = (unsigned int *) glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
	for( int i=0; i<12; i++ )
		std::cout << uptr[i] << " ";
	std::cout << std::endl;
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	
	GLuint vao = inst.VAO;
	glBindVertexArray(vao);
	
	int count = vii.size();
	int cind = inst.numIndicesPerInstance;
//std::cout << inst.VBO << " " << inst.IBO << " " << inst.InstBO << " " << inst.VAO << std::endl;
//std::cout << cind << " " << count << std::endl;
	glDrawElementsInstanced( GL_POINTS, cind, GL_UNSIGNED_INT, (void*)0, count );
//	glDrawElementsBaseVertex( GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)0, 0 );
	glBindVertexArray(0);
}

//Transpose = GL_TRUE because GLSL uses Column-Major where C++ typicall uses Row-Major
void InstRenderer::WireframeInst( const Inst & inst, const std::vector<InstInfo> & vii, const glm::vec4 & llb, const float & res ) {
	glUseProgram(theProgram);
	glUniform4f(uvar[0], llb[0], llb[1], llb[2], 0.0f);
	glUniform1f(uvar[1], res);
		
	glUniform4f(uvar[5], 0.5f, 0.5f, 0.0f, 1.0f);
	
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


