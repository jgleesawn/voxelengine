#include "inputstatehierarchy.h"
#include "device.h"
#include "state.h"

InputStateHierarchy::InputStateHierarchy() {
		addDevice<SDLKeyboard>();
		addDevice<SDLMouse>();
/*		da.push_back(new SDLKeyboard);
		da.push_back(new SDLMouse);
		da_mask.push_back(new SDLKeyboard);
		da_mask.push_back(new SDLMouse);
*/
	}
InputStateHierarchy::~InputStateHierarchy() {
	while(!da.empty()) {
		delete da.back();
		da.pop_back();
	}
	while(!da_mask.empty()) {
		delete da_mask.back();
		da_mask.pop_back();
	}
	while(!states.empty()) {
		delete states.back();
		states.pop_back();
	}
}

/*
void InputStateHierarchy::addDevice( Device * device_in ) {
	da.push_back(device_in);
	
}
*/
void InputStateHierarchy::pushState( State * state_in ) {
	states.push_back(state_in);
}
void InputStateHierarchy::popState() {
	delete states.back();
	states.pop_back();
}

void InputStateHierarchy::update() {
	for( int i=0; i<da.size(); i++ ) {
		da[i]->update();
		da_mask[i]->clear();
//		da_mask[i]->update();
	}
}

void InputStateHierarchy::processInputs() {
//	update();
	for( int i = states.size()-1; i>=0; i-- ) {
		states[i]->processInputs();
	}
}

void InputStateHierarchy::renderStates() {
	for( int i=0; i<states.size(); i++ )
		states[i]->Render();
}
