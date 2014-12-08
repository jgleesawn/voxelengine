#include "selection.h"

Selecting::Selecting() : selectionBox(glm::vec4(0.0f), 0) {
	g_obj.setCollisionShape(new btSphereShape(1.0f));
//	g_obj.setCollisionShape(new btBoxShape(btVector3(1.0, 1.0, 1.0)));
	g_obj.setWorldTransform(btTransform());
//	g_obj.setCollisionFlags( g_obj.getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE );
}

Selecting::~Selecting() {
	w->dynamicsWorld->removeCollisionObject(&g_obj);
}

void Selecting::PlaceStart(float x, float y) {
	std::pair<glm::vec4, glm::vec4> spair = view->getCloseFar(x, y);
	sclose = spair.first;
	sfar = spair.second;
	w->dynamicsWorld->addCollisionObject(&g_obj);

	if( g_obj.getCollisionShape()->isConvex() ) {
		btShapeHull sh((btConvexShape*)g_obj.getCollisionShape());
		Inst_t<ObjModel> * inst = new Inst_t<ObjModel>(sh);
		selectionBox.instance_id = glm->LoadInst(inst);
	}
}

//Currently does not render btGhostObject to show bounding box.
//Possibly introduce some type of scaling to a primitive or model renderer to accomodate this.
void Selecting::LoadScene() {
	for( int i=0; i<renObjs.size(); i++ )
		renObjs[i]->addRenderInfo(renderInfo);
}
void Selecting::Render() {
	renderInfo.clear();
	LoadScene();
//	Scene::Render();
	DebugRender();
}

#include <iostream>
State* Selecting::Update(int ddsize, float * ddata) {
	std::pair<glm::vec4, glm::vec4> npair = view->getCloseFar(ddata[0], ddata[1]);

	glm::vec4 smid = (sclose + sfar)/2.0f;
	glm::vec4 nmid = (npair.first + npair.second)/2.0f;
/*
	glm::vec4 bmin = glm::min(sclose, sfar);
	bmin = glm::min(bmin, npair.first);
	bmin = glm::min(bmin, npair.second);

	glm::vec4 bmax = glm::max(sclose, sfar);
	bmax = glm::max(bmax, npair.first);
	bmax = glm::max(bmax, npair.second);

	glm::vec4 bcenter = (bmin + bmax)/2.0f;
	glm::vec4 halfedge = bcenter - bmin;
//	printv(bcenter); printv(halfedge);
	glm::vec3 hedge(halfedge.x, halfedge.y, halfedge.z);
	glm::quat rotq = glm::rotation(glm::vec3(0.0f, 0.0f, -0.5f), hedge);
//	g_obj.getCollisionShape()->setLocalScaling(1.1*g_obj.getCollisionShape()->getLocalScaling());
	g_obj.getCollisionShape()->setLocalScaling(*(btVector3 *)&halfedge);
	g_obj.getWorldTransform().setOrigin(*(btVector3 *)&bcenter);
	g_obj.getWorldTransform().setRotation(*(btQuaternion *)&rotq);
*/
	glm::vec4 middiff = nmid-smid;
	g_obj.getCollisionShape()->setLocalScaling(*(btVector3*)&middiff);
	glm::vec4 avgmid = (smid + nmid)/2.0f;
	g_obj.getWorldTransform().setOrigin(*(btVector3 *)&avgmid);

	btTransform trans = g_obj.getWorldTransform();
//	trans.setRotation(btQuaternion(1.0f, 0.0f, 0.0f, 0.0f));
/*
	for( int i=0; i<3; i++ )
		std::cerr << trans.getOrigin()[i] << " ";
	std::cerr << std::endl;
*/
	selectionBox.setWorldTransform(g_obj.getWorldTransform());
//	printm(selectionBox.getQRotMat());
	if( g_obj.getCollisionShape()->isConvex() ) {
		btShapeHull sh((btConvexShape*)g_obj.getCollisionShape());
		Inst_t<ObjModel> * inst = new Inst_t<ObjModel>(sh);
		glm->RemoveInst(selectionBox.instance_id);
		selectionBox.instance_id = glm->LoadInst(inst);
	}


	btAlignedObjectArray<btCollisionObject*> obj_arr = g_obj.getOverlappingPairs();

	btRigidBody * rb;
	Object * obj;
	renObjs.clear();
	for( int i=0; i<obj_arr.size(); i++ ) {
		rb = (btRigidBody *) obj_arr[i];
		obj = (Object *)(rb->getMotionState());
		if( obj->getType() != 10 ) {
			continue;
		}
//		renObjs.push_back((Renderable *)obj);
	}
	renObjs.push_back(&selectionBox);
	return 0;
}
State* Selecting::Finish(int ddsize, float * ddata) {
	Selected * sed = new Selected;
	sed->setRen(w->ren);
	sed->setGLM(w->glm);
	sed->renObjs = renObjs;
	State * s_ret = new TState<Selected>(sed);
	return s_ret;
}


void Selected::LoadScene() {
	for( int i=0; i<renObjs.size(); i++ )
		renObjs[i]->addRenderInfo(renderInfo);
}
void Selected::Render() {
	renderInfo.clear();
	LoadScene();
	DebugRender();
}

State* Selected::Update(int ddsize, float * ddata) {
	return 0;
}
State* Selected::DeSelect(int ddsize, float * ddata) {
	return (State *)-2;
}
