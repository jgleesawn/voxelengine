#include "selection.h"

Selecting::Selecting() : selectionBox(glm::vec4(0.0f), 0) {
//	g_obj.setCollisionShape(new btSphereShape(1.0f));
	g_obj.setCollisionShape(new btBoxShape(btVector3(1.0, 1.0, 1.0)));
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
#include <typeinfo>
State* Selecting::Update(int ddsize, float * ddata) {
	std::pair<glm::vec4, glm::vec4> npair = view->getCloseFar(ddata[0], ddata[1]);

	glm::vec4 smid = (sclose + sfar)/2.0f;
	glm::vec4 nmid = (npair.first + npair.second)/2.0f;
	glm::vec4 avgmid = (smid + nmid)/2.0f;
	
	smid.w = 0.0f;
	nmid.w = 0.0f;
	glm::vec4 vdir = nmid-smid;
	glm::vec3 v3dir(vdir.x,vdir.y,vdir.z);
//	btVector3 bscale(1.0f, 1.0f, glm::length2(v3dir));
//	glm::vec3 vscale = *(glm::vec3 *)&bscale; //(1.0f, 1.0f, glm::length(v3dir));
	btVector3 bscale = g_obj.getCollisionShape()->getLocalScaling() * btVector3(1.0f, 1.0f, 1.01f);
//	bscale.setZ(glm::length2(v3dir));
	g_obj.getCollisionShape()->getLocalScaling();
	glm::vec4 vscale = *(glm::vec4 *)&bscale;
	vscale[2] = (double)glm::length(v3dir);
	if( vscale[2] < 0.2f )
		vscale[2] = 0.2f;
	std::cerr << "Scalar vector." << std::endl;
	printv(vscale);

	v3dir = glm::normalize(v3dir);
	glm::quat rotq = glm::rotation(glm::vec3(0.0f, 0.0f, -1.0f), v3dir);
	g_obj.getCollisionShape()->setLocalScaling(*(btVector3 *)&vscale);
	g_obj.getWorldTransform().setOrigin(*(btVector3 *)&avgmid);
	btQuaternion q;
	q.setX(rotq.x); q.setY(rotq.y); q.setZ(rotq.z); q.setW(rotq.w);
	g_obj.getWorldTransform().setRotation(*(btQuaternion *)&rotq);
	
	btTransform trans = g_obj.getWorldTransform();
	selectionBox.setWorldTransform(g_obj.getWorldTransform());

	if( g_obj.getCollisionShape()->isConvex() ) {
		std::cerr << "Recompute Hull." << std::endl;
		btVector3 bscale = g_obj.getCollisionShape()->getLocalScaling();
		for( int i=0; i<4; i++ )
			std::cerr << bscale[i] << " ";
		std::cerr << std::endl;
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
