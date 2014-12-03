#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <iostream>

#include "sparseworld/world.h"
#include "object/viewport.h"
#include "interface/inputstatehierarchy.h"
#include "interface/state.h"

#include "scenes/tempselection.h"

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

class World;

struct Controller {
	int object_id;
	World * w;
	glm::mat4 perspectiveMatrix;
//	Viewport view;

	InputStateHierarchy ISH;

	Controller(World *, int object_id_in = 0);
	void PrepScene();
	void LoadScene();
	void Render();

	State* MoveFocusForward(int, float*);
	State* MoveFocusLeft(int, float*);
	State* MoveFocusRight(int, float*);
	State* MoveFocusBack(int, float*);

	State* RotFocusRight(int, float*);
	State* RotFocusLeft(int, float*);
	State* RotFocusUp(int, float*);
	State* RotFocusDown(int, float*);

//HAS MEMORY LEAK
	State* raySelect(int, float*);

};


#endif
