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

/*
template <>
TState<Selection::TState(InputStateHierarchy * ISH_in, Selecting * obj_in) {
}
*/

