#ifndef SELECTION_H
#define SELECTION_H

#include "scene.h"
#include "interface/state.h"
#include "sparseworld/world.h"
#include "object/viewport.h"
#include "object/renderable.h"

#include <btBulletCollisionCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include <glm/gtx/quaternion.hpp>

struct Selecting : public Scene<ObjModel> {
	Viewport * view;
	World * w;

//	btVector3 bstart;
	glm::vec4 sclose, sfar;

	btGhostObject g_obj;
	std::vector<Renderable *> renObjs;

	Renderable selectionBox;

	Selecting();
	~Selecting();
	void PlaceStart(float, float);
	virtual void LoadScene();
	virtual void Render();

	State* Update(int, float*);
	State* Finish(int, float*);
};

struct Selected : public Scene<ObjModel> {
	std::vector<Renderable *> renObjs;

	virtual void LoadScene();
	virtual void Render();

	State* Update(int, float*);
	State* DeSelect(int, float*);
};



#endif


