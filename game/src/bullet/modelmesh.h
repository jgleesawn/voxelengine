#ifndef MODELMESH_H
#define MODELMESH_H


#include <btBulletDynamicsCommon.h>

#include "glmanager/objmodel.h"

#include <iostream>


struct ModelMesh {
	ObjModel * om;
	btIndexedMesh * im;
	btTriangleIndexVertexArray * tiva;

	ModelMesh(ObjModel *);
	ModelMesh(ModelMesh *);
	~ModelMesh();

	ObjModel * release();
};

#endif
