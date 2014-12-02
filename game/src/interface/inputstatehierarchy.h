#ifndef INPUTSTATEHIERARCHY_H
#define INPUTSTATEHIERARCHY_H

#include <vector>

class Device;
struct State;

class InputStateHierarchy {
public:
	std::vector< Device * > da;
	std::vector< Device * > da_mask;
	std::vector< State * > states;

	InputStateHierarchy();
	~InputStateHierarchy();
	template<typename T>
	void addDevice();
	void addState( State * state_in );

	void update();

	void processInputs();
};

template<typename T>
void InputStateHierarchy::addDevice() {
	da.push_back(new T);
	da_mask.push_back(new T);
}

#endif
