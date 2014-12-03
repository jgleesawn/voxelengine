#ifndef TEMPSELECTION_H
#define TEMPSELECTION_H

#include "scene.h"
#include "object/renderable.h"

struct TempSelection : public Scene<ObjModel> {
	std::vector<Renderable *> renObjs;
	State* Dummy(int, float*) { return 0; }

	virtual void Render() {
		DebugRender();
	}
	virtual void LoadScene() {
		for( int i=0; i<renObjs.size(); i++ )
			renObjs[i]->addRenderInfo(renderInfo);
	}
};


#endif
