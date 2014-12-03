#include "world.h"

#include <iostream>

World::World() {
	terrain = new Terrain(this);
	focus = 0;	//camera

	broadphase = new btDbvtBroadphase();
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

	dynamicsWorld->setGravity(btVector3(0, -10, 0));
/*
	btCollisionShape * groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), -10);
	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
	btRigidBody::btRigidBodyConstructionInfo* groundRigidBodyCI = new btRigidBody::btRigidBodyConstructionInfo(0, groundMotionState, groundShape, btVector3(0, 0, 0));
	btRigidBody* groundRigidBody = new btRigidBody(*groundRigidBodyCI);

	groundRigidBody->setRestitution(1.0f);

	dynamicsWorld->addRigidBody(groundRigidBody);
*/
//	cloud->width = 0;
//	cloud->height = 1;
//	cloud->points.resize(0);
//	octree.setInputCloud(cloud);
}

World::~World() {
	for( int i=0; i<objects.size(); i++ ) {
		if( objects[i] ) {
			dynamicsWorld->removeCollisionObject(objects[i]->rigidBody);
			delete objects[i];
		}
	}

}

int World::addObject( Object * obj, const glm::vec4 & pos ) {
//	cloud->push_back(*((pcl::PointXYZ *)(&(obj->position[0]))));
//	btRigidBody::btRigidBodyConstructionInfo RigidBodyCI(obj->mass, obj, obj->shape, obj->inertia);
	dynamicsWorld->addRigidBody(obj->rigidBody);
	objects.push_back(obj);
	obj->index = objects.size()-1;//cloud->size()-1;
	return obj->index;
}

void World::removeObject(int ind) {
	if( objects[ind] ) {
		dynamicsWorld->removeCollisionObject( objects[ind]->rigidBody );
		delete objects[ind];
		objects[ind] = NULL;
	}
}

int World::makeRenderable( int ind ) {
	renObjs.push_back( (Renderable *) objects[ind]);
	return renObjs.size()-1;
}

void World::update() {
	dynamicsWorld->stepSimulation(1 /60.0f, 10);
//	octree.deleteTree();
//	octree.addPointsFromInputCloud();
}

void World::MoveObject( Object * obj, const glm::vec4 & offset ) {
	obj->Move(offset);
//	*(glm::vec4 *)&(cloud->points[obj->index]) += offset;
}

State* World::MoveFocusForward(int ddsize, float* ddata) {
	objects[focus]->Move(0.5f*objects[focus]->getForward());
	return 0;
}

State* World::MoveFocusRight(int ddsize, float* ddata) {
	objects[focus]->Move(0.5f*objects[focus]->getRight());
	return 0;
}

State* World::MoveFocusLeft(int ddsize, float* ddata) {
	objects[focus]->Move(-0.5f*objects[focus]->getRight());
	return 0;
}

State* World::MoveFocusBack(int ddsize, float* ddata) {
	objects[focus]->Move(-0.5f*objects[focus]->getForward());
	return 0;
}

State* World::RotFocusRight(int ddsize, float* ddata) {
	objects[focus]->rotY(-.05);
	return 0;
}

State* World::RotFocusLeft(int ddsize, float* ddata) {
	objects[focus]->rotY(.05);
	return 0;
}

State* World::RotFocusUp(int ddsize, float* ddata) {
	objects[focus]->rotX(.05);
	return 0;
}

State* World::RotFocusDown(int ddsize, float* ddata) {
	objects[focus]->rotX(-.05);
	return 0;
}

State* World::focusCamera(int ddsize, float* ddata) {
	btTransform trans;
	objects[focus]->getWorldTransform(trans);
	objects[camera]->setWorldTransform(trans);
	focus = camera;
//	printv(*(glm::vec4 *)&(cloud->points[focus]));
	return 0;
}

State* World::focusNext(int ddsize, float* ddata) {
	focus++;
	if( focus > objects.size() )
		focus = 0;
//	printv(*(glm::vec4 *)&(cloud->points[focus]));
	return 0;
}
/* //Move to Controller class
State* World::raySelect(int ddsize, float* ddata) {
	btVector3 start, end;
	std::pair<glm::vec4, glm::vec4> p = view->getCloseFar(ddata[0], ddata[1]);
	start = *(btVector3 *)&p.first;
	end = *(btVector3 *)&p.second;

	btCollisionWorld::AllHitsRayResultCallback crr_callback(start, end);
	dynamicsWorld->rayTest(start, end, crr_callback);

	btRigidBody * rb;
	Object * obj;
	for( int i=0; i<crr_callback.m_collisionObjects.size(); i++ ) {
		rb = (btRigidBody *) crr_callback.m_collisionObjects[i];
		obj = (Object *)(rb->getMotionState());
		if( obj->getType() != 10 0 {
			continue;
		}
		((Renderable *)obj)->addRenderInfo(renderInfo);
	}
	return 0;
}
*/

void World::Wiggle() {
	for( int i=0; i<renObjs.size(); i++ ) {
		MoveObject( renObjs[i], glm::vec4( (float)rand()/RAND_MAX - .5, (float)rand()/RAND_MAX - .5, (float)rand()/RAND_MAX - .5, 0.0f ) );
	}
}






