#ifndef BULLETBVH_H
#define BULLETBVH_H

#include <btBulletDynamicsCommon.h>

#include "modelmesh.h"

struct BvhTriangleMeshShapeContainer : public btBvhTriangleMeshShape {
	btVector3 minbb, maxbb;
	ModelMesh mm;
	BvhTriangleMeshShapeContainer(const ModelMesh & );
};




#endif
