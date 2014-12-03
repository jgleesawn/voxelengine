#include "state.h"

#include "../sparseworld/world.h"

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
TState<Controller>::TState(InputStateHierarchy * ISH_in, Controller * obj_in) {
	ISH = ISH_in;
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
}

//add source folder scenes and group derived classes there.
//struct Selection : public Scene {};
/*
template<>
TState<Selection>::TState(InputStateHierarchy * ISH_in, Selection * obj_in) {
	ISH = ISH_in;
	obj = obj_in;
	setInput(&Selection::Dummy, 1, 1, 1, 0);
}
*/
template<>
TState<TempSelection>::TState(InputStateHierarchy * ISH_in, TempSelection * obj_in) {
	ISH = ISH_in;
	obj = obj_in;
	setInput(&TempSelection::Dummy, 0, (int)SDL_SCANCODE_TAB, 0, 1);
}
/*
template <>
TState<Selection::TState(InputStateHierarchy * ISH_in, Selecting * obj_in) {
}
*/

