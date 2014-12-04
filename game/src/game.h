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
#include "interface/inputstatehierarchy.h"
#include "interface/state.h"
#include "interface/device.h"

#include "sparseworld/world.h"

#include "controllers/controller.h"

class Game {
	
public:
	GLmanager<ObjModel> glm;
	InstRenderer<ObjModel> * ren;
	Viewport * view;
	World w;

	std::vector<Controller *> players;
//	InputStateHierarchy ISH;

	Game();
	~Game();

	void Loop();
};


#endif



