#ifndef INSTRENDERER_H
#define INSTRENDERER_H

//#include <GL/glew.h>
#include <string>
#include <vector>

#include <iostream>

#include "object/renderable.h"
#include "object/viewport.h"
#include "glprogrambase.h"
#include "glmanager.h"
#include "basicrenderer.h"

//May have to change Render function to virtual.

class InstRenderer : public BasicRenderer {
public:
	virtual void Initialize();
	InstRenderer();
	~InstRenderer() { }//glDeleteProgram(theProgram); }

	//void setGLM(GLmanager *);

	//void setCameraRotationPerspective(const glm::mat4 &, const glm::mat4 & );
	//void setCameraPos(const glm::vec4 &);

	void RenderInst(const Inst &, const std::vector<InstInfo> &, const glm::vec4 &, const float &);
	void WireframeInst(const Inst &, const std::vector<InstInfo> &, const glm::vec4 &, const float &);

//	virutal void Render(Renderable *, const glm::vec4 &);
//	virtual void Wireframe(Renderable *, const glm::vec4 &);

	//void reshape(int, int);
};


#endif
