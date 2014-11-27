#ifndef WORLD_H
#define WORLD_H

//Hack of a fix
//#undef Success
//#include <pcl/point_cloud.h>
//#include <pcl/octree/octree.h>

#include <iostream>
#include <btBulletDynamicsCommon.h>

#include "terrain.h"
#include "object/object.h"
#include "object/renderable.h"

class Terrain;

class World {
public:
	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;

	std::vector<Object *> objects;
	std::vector<Renderable *> renObjs;
	Terrain * terrain;

	std::vector<int> selection;

	int camera, focus;

public:
	World();
	~World();
	int addObject( Object *, const glm::vec4 & pos );
	int makeRenderable( int );
	void update();

	void MoveObject( Object *, const glm::vec4 & );
	
	void MoveFocusForward();
	void MoveFocusLeft();
	void MoveFocusRight();
	void MoveFocusBack();

	void RotFocusRight();
	void RotFocusLeft();
	void RotFocusUp();
	void RotFocusDown();

	void focusCamera();
	void focusNext();

	void Wiggle();	//remove
};


#endif
