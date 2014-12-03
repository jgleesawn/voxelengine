#ifndef STATE_H
#define STATE_H

#include <map>
#include <array>
#include "inputstatehierarchy.h"
#include "device.h"
#include "interface.h"	//For sorting of a member function

struct State {
	InputStateHierarchy * ISH;
	virtual void processInputs() = 0;
	virtual void Render() = 0;
};

template <typename T>
struct TState : public State {
	typedef State* (T::*tfptr_t)(int, float*);
	T* obj;
	std::map<tfptr_t, std::array<int, 4>, std::less<tfptr_t const> > m;

	TState(InputStateHierarchy * ISH_in, T * obj_in) {
		ISH = ISH_in;
		obj = obj_in;
	}
	void setInput(tfptr_t tfptr, int dev, int ind, int check_for, int pass_dev) {
		m[tfptr] = std::array<int, 4>{dev,ind,check_for,pass_dev};
	}
	void remInput(tfptr_t tfptr) {
		m.erase(tfptr);
	}
	virtual void processInputs() {
		typename std::map<tfptr_t, std::array<int, 4> >::iterator it;
		for( it = m.begin(); it != m.end(); it++ ) {
			int dev = it->second[0];
			int ind = it->second[1];
			int check_for = it->second[2];
			int pass_dev = it->second[3];
			if( !(ISH->da_mask[dev]->data[ind] || (ISH->da[dev]->data[ind] ^ check_for)) ) {
//			if( !(ISH->da[dev]->data[ind] ^ check_for) ) {
				State * n_state = (obj->*(it->first))(ISH->da[pass_dev]->vsize, ISH->da[pass_dev]->vals);
				if( n_state )
					ISH->pushState(n_state);
//				ISH->da[dev]->data[ind] = 0;
//				ISH->da_mask[dev]->data[ind] = 1;
			}
			ISH->da_mask[dev]->data[ind] = 1;
		}
	}
	virtual void Render() {
//Uncomment when Scene is implemented as a base class.
//		obj->Render();
	}
};


#endif
