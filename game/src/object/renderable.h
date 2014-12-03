#ifndef RENDERABLE_H
#define RENDERABLE_H

#include "object.h"
#include "glmanager/glmanager.h"

class Renderable : public Object {
public:
	int instance_id;
//	gfxObj_t gfxObj;	//Removed usage when instancing was added. possibly use both, we will see.
	Renderable(glm::vec4, int, glm::quat or_in = glm::quat(), float p_in=0.0f, float y_in=0.0f, float m_in=10.0f, btCollisionShape * shape_in = new btSphereShape(1));

	void addRenderInfo(std::map<int, std::vector<InstInfo<ObjModel> > > &);
};


#endif
