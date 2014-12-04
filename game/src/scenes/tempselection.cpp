#include "tempselection.h"

State* TempSelection::Finish(int ddsize, float * ddata) {
	delete this;
	return (State *)-1;
}

State* TempSelection::Update(int ddsize, float * ddata) {
	std::cerr << "TempSelection::Update" << std::endl;
	btVector3 start, end;
	std::pair<glm::vec4, glm::vec4> p = view->getCloseFar(ddata[0], ddata[1]);
	start = *(btVector3 *)&p.first;
	end = *(btVector3 *)&p.second;

//	start = btVector3(3.0f, -100.0f, 3.0f);
//	end = btVector3(3.0f, 50.0f, 3.0f);

	btCollisionWorld::AllHitsRayResultCallback crr_callback(start, end);
	w->dynamicsWorld->rayTest(start, end, crr_callback);

	btRigidBody * rb;
	Object * obj;
	renObjs.clear();
	for( int i=0; i<crr_callback.m_collisionObjects.size(); i++ ) {
		rb = (btRigidBody *) crr_callback.m_collisionObjects[i];
		obj = (Object *)(rb->getMotionState());
		if( obj->getType() != 10 ) {
			continue;
		}
		renObjs.push_back((Renderable *)obj);
	}
	return 0;
}	
	
