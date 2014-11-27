#include "modelmesh.h"

ModelMesh::ModelMesh(ObjModel * om_in) {
	if( !om_in ) {
		om = NULL;
		im = NULL;
		tiva = NULL;
		return;
	}
	om = om_in;
	im = new btIndexedMesh;
	tiva = new btTriangleIndexVertexArray;

	im->m_numTriangles = om->ibo.size()/3;
	im->m_triangleIndexBase = (unsigned char *)om->ibo.data();
	im->m_triangleIndexStride = 3*sizeof(int);
	im->m_numVertices = om->vbo.size();
	im->m_vertexBase = (unsigned char *)om->vbo.data();
	im->m_vertexStride = 4*sizeof(float);

	tiva->addIndexedMesh(*im);
}
ModelMesh::ModelMesh(ModelMesh * mm_in) {
	om = mm_in->om;
	im = mm_in->im;
	tiva = mm_in->tiva;
}

ModelMesh::~ModelMesh() {
	if( om ) {
		delete om;
		om = NULL;
	}
	if( im ) {
		delete im;
		im = NULL;
	}
	if( tiva ) {
		delete tiva;
		tiva = NULL;
	}
}

ObjModel * ModelMesh::release() {
	ObjModel * out = om;
	om = NULL;
	im = NULL;
	tiva = NULL;
	return out;
}
