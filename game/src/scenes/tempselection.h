#ifndef TEMPSELECTION_H
#define TEMPSELECTION_H

#include <iostream>

#include "scene.h"
#include "object/renderable.h"
#include "sparseworld/world.h"

class World;

struct TempSelection : public Scene<ObjModel> {
	Viewport * view;
	World * w;

	std::vector<Renderable *> renObjs;
	State* Dummy(int, float*) { return 0; }
	State* Finish(int, float*);
	State* Update(int, float*);

	virtual void Render() {
		renderInfo.clear();
		LoadScene();
		DebugRender();
	}
	virtual void LoadScene() {
		for( int i=0; i<renObjs.size(); i++ )
			renObjs[i]->addRenderInfo(renderInfo);
	}
};


#endif
