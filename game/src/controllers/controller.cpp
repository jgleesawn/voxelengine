#include "controller.h"

Controller::Controller(World * w_in, int object_id_in) : w(w_in), object_id(object_id_in), ISH(new TState<Controller>(this)) {
	perspectiveMatrix = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
//	ISH.pushState(new TState<Controller>(this));
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
//	std::cerr << "Controller::raySelect" << std::endl;
	TempSelection * tselect = new TempSelection();
	tselect->setRen(w->ren);
	tselect->setGLM(w->glm);
	tselect->view = (Viewport *)w->objects[object_id];
	tselect->w = w;
	State * s_ret = new TState<TempSelection>(tselect);
	return s_ret;
}

//Uses Mouse Input
//Abuses the fact that Controller object has been passed a Viewport
State* Controller::StartSelecting(int ddsize, float* ddata) {
	Selecting * sing = new Selecting;
	sing->setRen(w->ren);
	sing->setGLM(w->glm);
	sing->view = (Viewport *)w->objects[object_id];
	sing->w = w;
	sing->PlaceStart(ddata[0], ddata[1]);
	State * s_ret = new TState<Selecting>(sing);
	return s_ret;
}






