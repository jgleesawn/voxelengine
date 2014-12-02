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
	void removeObject(int);

	int makeRenderable( int );
	void update();

	void MoveObject( Object *, const glm::vec4 & );
	
	void MoveFocusForward(int, int*);
	void MoveFocusLeft(int, int*);
	void MoveFocusRight(int, int*);
	void MoveFocusBack(int, int*);

	void RotFocusRight(int, int*);
	void RotFocusLeft(int, int*);
	void RotFocusUp(int, int*);
	void RotFocusDown(int, int*);

	void focusCamera(int, int*);
	void focusNext(int, int*);

	void Wiggle();	//remove
};


#endif
