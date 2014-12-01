#ifndef GAME_H
#define GAME_H

#include <string>
#include <map>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <SDL2/SDL_mouse.h>

#include <vector>

//#include <bullet/BulletDynamics/Character/btKinematicCharacterController.h>
//class btKinematicClosestNotMeRayResultCallback;

#include "glmanager/glengine.h"
#include "glmanager/instrenderer.h"
#include "interface/interface.h"

#include "sparseworld/world.h"

class Game {
	
public:
	GLmanager glm;
	InstRenderer * ren;
	Viewport * view;
	World w;

	std::vector<input> inputs;

	std::vector<int> instance_ids;
//	std::vector<gfxObj_t> gfxObjs;
//	std::vector<Renderable *> renObjs;

//	Interface<Viewport> interface;
	Interface<World> interface;

	Game();
	~Game();
	void addInput(const void *, int, int);
	void Loop();
};


#endif



