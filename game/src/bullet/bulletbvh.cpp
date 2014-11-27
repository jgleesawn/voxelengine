#include "bulletbvh.h"

BvhTriangleMeshShapeContainer::BvhTriangleMeshShapeContainer( 
		const ModelMesh & mm_in) : mm(mm_in), 
		btBvhTriangleMeshShape(mm_in.tiva, 1) { //, minbb, maxbb) {
}

