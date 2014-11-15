#include "basicrenderer.h"

void BasicRenderer::Initialize() {
	std::vector<shaderName> shaderNames;
	shaderNames.push_back(shaderName(GL_VERTEX_SHADER, std::string("renderers/basic.v.shader")));
//	shaderNames.push_back(shaderName(GL_GEOMETRY_SHADER, std::string("renderers/basic.g.shader")));
	shaderNames.push_back(shaderName(GL_FRAGMENT_SHADER, std::string("renderers/basic.f.shader")));
	theProgram = GLProgramBase().InitializeProgram(shaderNames);

	std::cout << theProgram << std::endl;

	glUseProgram(theProgram);

	uvar[0] = glGetUniformLocation( theProgram, "objPosition");
	uvar[1] = glGetUniformLocation( theProgram, "objRotation");
	uvar[2] = glGetUniformLocation( theProgram, "viewOffset");
	uvar[3] = glGetUniformLocation( theProgram, "viewRotation");
	uvar[4] = glGetUniformLocation( theProgram, "Perspective");
	uvar[5] = glGetUniformLocation( theProgram, "uColor");

	for( int i=0; i<numUniforms; i++ )
		std::cout << uvar[i] << std::endl;
	glUseProgram(0);
}

//Need Init function so that the GL context is set up already;
//VE is created before GL context is set up.
BasicRenderer::BasicRenderer(int numU) : glm(NULL), numUniforms(numU) {
	uvar = new int[numU];
	Initialize();	//Call this as it is virtual, can specify on a per-renderer basis.
}

void BasicRenderer::setGLM(GLmanager * glm_in) {
	glm = glm_in;
}

void BasicRenderer::setCameraRotationPerspective(const glm::mat4 & Rotation, const glm::mat4 & Perspective) {
	glm::mat4 rotpersp = Perspective*Rotation;
//Transposes matrix on glImport so multiplication must be reversed before transpose.
	glUniformMatrix4fv(uvar[3], 1, GL_TRUE, &rotpersp[0][0]);
	glUniformMatrix4fv(uvar[4], 1, GL_TRUE, &glm::mat4(1.0f)[0][0]);

//	glUniformMatrix4fv(uvar[3], 1, GL_TRUE, &Rotation[0][0]);
//	glUniformMatrix4fv(uvar[4], 1, GL_TRUE, &Perspective[0][0]);
}

void BasicRenderer::setCameraPos( const glm::vec4 & vpos ) {
	glUniform4f(uvar[2], vpos[0], vpos[1], vpos[2], 0.0f );
}

//Transpose = GL_TRUE because GLSL uses Column-Major where C++ typicall uses Row-Major
void BasicRenderer::Render( Renderable * objs, const glm::vec4 & op ) {
	glm::mat4 orotMat = glm::toMat4(objs->orientation);
	
	glUseProgram(theProgram);
	glUniform4f(uvar[0], op[0], op[1], op[2], 0.0f);
	glUniformMatrix4fv(uvar[1], 1, GL_TRUE, &orotMat[0][0]);
	
	glUniform4f(uvar[5], 0.5f, 0.5f, 0.0f, 1.0f);
	
	gfxObj_t goID = objs->gfxObj;
	
	GLuint vao = glm->gfxObjs.at(goID.vao)->VAO;
	int ind = goID.ind;
	
	glBindVertexArray(vao);
	
	int count = glm->gfxObjs.at(goID.vao)->IBOStartingIndices.at(goID.ind+1) - glm->gfxObjs.at(goID.vao)->IBOStartingIndices.at(goID.ind);
	int indoffset = glm->gfxObjs.at(goID.vao)->IBOStartingIndices.at(goID.ind) * glm->gfxObjs.at(goID.vao)->ibo_struct_size;
	int baseoffset = glm->gfxObjs.at(goID.vao)->VBOStartingIndices.at(goID.ind);
	
	glDrawElementsBaseVertex( GL_TRIANGLES, count, GL_UNSIGNED_INT, (void*)indoffset, baseoffset);
}

void BasicRenderer::Wireframe( Renderable * objs, const glm::vec4 & op ) {
	glm::mat4 orotMat = glm::toMat4(objs->orientation);
	
	glUseProgram(theProgram);
	glUniform4f(uvar[0], op[0], op[1], op[2], 0.0f);
	glUniformMatrix4fv(uvar[1], 1, GL_TRUE, &orotMat[0][0]);
	
	glUniform4f(uvar[5], 0.0f, 1.0f, 0.0f, 1.0f);
	
	gfxObj_t goID = objs->gfxObj;
	
	GLuint vao = glm->gfxObjs.at(goID.vao)->VAO;
	int ind = goID.ind;
	
	glBindVertexArray(vao);
	
	int count = glm->gfxObjs.at(goID.vao)->IBOStartingIndices.at(goID.ind+1) - glm->gfxObjs.at(goID.vao)->IBOStartingIndices.at(goID.ind);
	int indoffset = glm->gfxObjs.at(goID.vao)->IBOStartingIndices.at(goID.ind) * glm->gfxObjs.at(goID.vao)->ibo_struct_size;
	int baseoffset = glm->gfxObjs.at(goID.vao)->VBOStartingIndices.at(goID.ind);
	
	glDrawElementsBaseVertex( GL_LINE_STRIP, count, GL_UNSIGNED_INT, (void*)indoffset, baseoffset);
}

void reshape (int w, int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

