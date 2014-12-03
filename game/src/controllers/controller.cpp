#include "controller.h"

Controller::Controller(World * w_in, int object_id_in) : w(w_in), object_id(object_id_in) {
	perspectiveMatrix = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
	ISH.pushState(new TState<Controller>(&ISH, this));
}

void Controller::PrepScene() {
	w->ren->Use();
	w->ren->setCameraRotationPerspective( w->objects[object_id]->getRotMat(), perspectiveMatrix);
	w->ren->setCameraPos(w->objects[object_id]->getPosition());
}
void Controller::LoadScene() {
	w->ClearScene();
	w->LoadScene();
}
void Controller::Render() {
	PrepScene();
	LoadScene();
	w->Render();
}

State* Controller::MoveFocusForward(int ddsize, float* ddata) {
	w->objects[object_id]->Move(0.5f*w->objects[object_id]->getForward());
	return 0;
}

State* Controller::MoveFocusRight(int ddsize, float* ddata) {
	w->objects[object_id]->Move(0.5f*w->objects[object_id]->getRight());
	return 0;
}

State* Controller::MoveFocusLeft(int ddsize, float* ddata) {
	w->objects[object_id]->Move(-0.5f*w->objects[object_id]->getRight());
	return 0;
}

State* Controller::MoveFocusBack(int ddsize, float* ddata) {
	w->objects[object_id]->Move(-0.5f*w->objects[object_id]->getForward());
	return 0;
}

State* Controller::RotFocusRight(int ddsize, float* ddata) {
	w->objects[object_id]->rotY(-.05);
	return 0;
}

State* Controller::RotFocusLeft(int ddsize, float* ddata) {
	w->objects[object_id]->rotY(.05);
	return 0;
}

State* Controller::RotFocusUp(int ddsize, float* ddata) {
	w->objects[object_id]->rotX(.05);
	return 0;
}

State* Controller::RotFocusDown(int ddsize, float* ddata) {
	w->objects[object_id]->rotX(-.05);
	return 0;
}

//Abuses the fact that Controller object has been passed a Viewport
State* Controller::raySelect(int ddsize, float* ddata) {
	btVector3 start, end;
	std::pair<glm::vec4, glm::vec4> p = ((Viewport *)&(w->objects[object_id]))->getCloseFar(ddata[0], ddata[1]);
	start = *(btVector3 *)&p.first;
	end = *(btVector3 *)&p.second;

	btCollisionWorld::AllHitsRayResultCallback crr_callback(start, end);
	w->dynamicsWorld->rayTest(start, end, crr_callback);

//	State * tselect = new TState<TempSelection>(ISH);
	TempSelection * tselect = new TempSelection();
	tselect->setRen(w->ren);
	tselect->setGLM(w->glm);
	btRigidBody * rb;
	Object * obj;
	for( int i=0; i<crr_callback.m_collisionObjects.size(); i++ ) {
		rb = (btRigidBody *) crr_callback.m_collisionObjects[i];
		obj = (Object *)(rb->getMotionState());
		if( obj->getType() != 10 ) {
			continue;
		}
		tselect->renObjs.push_back((Renderable *)obj);
//		((Renderable *)obj)->addRenderInfo(renderInfo);
	}
	return new TState<TempSelection>(&ISH, tselect);
}






