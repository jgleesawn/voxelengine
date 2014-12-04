#include "state.h"

#include "../sparseworld/world.h"

//Moving from this Constructor format.  Just haven't updated/used World state
template <>
TState<World>::TState(InputStateHierarchy * ISH_in, World * obj_in) {
	ISH = ISH_in;
	obj = obj_in;
	setInput(&World::MoveFocusForward, 0, (int)SDL_SCANCODE_W, 1, 0);
	setInput(&World::MoveFocusLeft, 0, (int)SDL_SCANCODE_A, 1, 0);
	setInput(&World::MoveFocusRight, 0, (int)SDL_SCANCODE_D, 1, 0);
	setInput(&World::MoveFocusBack, 0, (int)SDL_SCANCODE_S, 1, 0);

	setInput(&World::RotFocusLeft, 0, (int)SDL_SCANCODE_RIGHT, 1, 0);
	setInput(&World::RotFocusRight, 0, (int)SDL_SCANCODE_LEFT, 1, 0);
	setInput(&World::RotFocusUp, 0, (int)SDL_SCANCODE_UP, 1, 0);
	setInput(&World::RotFocusDown, 0, (int)SDL_SCANCODE_DOWN, 1, 0);

	setInput(&World::focusCamera, 0, (int)SDL_SCANCODE_SPACE, 1, 0);
//	setInput(&World::focusCamera, 0, (int)SDL_SCANCODE_TAB, 1, 0);

//Move to Controller class.  Should have access to view matrix and world class.
//	setInput(&World::raySelect, 0, (int)SDL_SCANCODE_TAB, 1, 1);
}

template<>
TState<Controller>::TState(Controller * obj_in ) {
	ISH = 0;
	obj = obj_in;
	setInput(&Controller::MoveFocusForward, 0, (int)SDL_SCANCODE_W, 1, 0);
	setInput(&Controller::MoveFocusLeft, 0, (int)SDL_SCANCODE_A, 1, 0);
	setInput(&Controller::MoveFocusRight, 0, (int)SDL_SCANCODE_D, 1, 0);
	setInput(&Controller::MoveFocusBack, 0, (int)SDL_SCANCODE_S, 1, 0);

	setInput(&Controller::RotFocusLeft, 0, (int)SDL_SCANCODE_RIGHT, 1, 0);
	setInput(&Controller::RotFocusRight, 0, (int)SDL_SCANCODE_LEFT, 1, 0);
	setInput(&Controller::RotFocusUp, 0, (int)SDL_SCANCODE_UP, 1, 0);
	setInput(&Controller::RotFocusDown, 0, (int)SDL_SCANCODE_DOWN, 1, 0);

	setInput(&Controller::raySelect, 0, (int)SDL_SCANCODE_TAB, 1, 1);
	setInput(&Controller::StartSelecting, 1, (int)SDL_BUTTON_LEFT, 1, 1);
}

template<>
TState<TempSelection>::TState(TempSelection * obj_in ) {
	ISH = 0;
	obj = obj_in;
	setInput(&TempSelection::Finish, 0, (int)SDL_SCANCODE_TAB, 0, 1);
	setInput(&TempSelection::Update, 0, (int)SDL_SCANCODE_TAB, 1, 1);
}
template <>
TState<Selecting>::TState(Selecting * obj_in ) {
	ISH = 0;
	obj = obj_in;
	setInput(&Selecting::Update, 1, (int)SDL_BUTTON_LEFT, 1, 1);//LMB down
	setInput(&Selecting::Finish, 1, (int)SDL_BUTTON_LEFT, 0, 1);//LMB up
}

template<>
TState<Selected>::TState(Selected * obj_in ) {
	ISH = 0;
	obj = obj_in;
	setInput(&Selected::Update, 1, (int)SDL_BUTTON_LEFT, 0, 1);//LMB up
	setInput(&Selected::DeSelect, 1, (int)SDL_BUTTON_LEFT, 1, 1);//LMB down
}

/*
template <>
TState<Selection::TState(InputStateHierarchy * ISH_in, Selecting * obj_in) {
}
*/

