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

#include "interface/state.h"

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
	
	State* MoveFocusForward(int, float*);
	State* MoveFocusLeft(int, float*);
	State* MoveFocusRight(int, float*);
	State* MoveFocusBack(int, float*);

	State* RotFocusRight(int, float*);
	State* RotFocusLeft(int, float*);
	State* RotFocusUp(int, float*);
	State* RotFocusDown(int, float*);

	State* focusCamera(int, float*);
	State* focusNext(int, float*);

//Pass XY in
//Move to Controller class
//	State* raySelect(int, float*);

	void Wiggle();	//remove
};


#endif
