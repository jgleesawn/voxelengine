#ifndef STATE_H
#define STATE_H

#include <map>
#include <array>
#include "inputstatehierarchy.h"
#include "device.h"
#include "interface.h"	//For sorting of a member function

//Headers for state types.
#include "sparseworld/world.h"
#include "controllers/controller.h"
#include "scenes/selection.h"
#include "scenes/tempselection.h"

struct State {
	InputStateHierarchy * ISH;
	virtual void processInputs() = 0;
	virtual void Render() = 0;

	void setISH(InputStateHierarchy * ISH_in) { ISH = ISH_in; }
	void exit() {
		while( ISH->states.back() != this && !ISH->states.empty() ) {
			delete ISH->states.back();
			ISH->states.pop_back();
		}
		if( !ISH->states.empty() ) {
			delete ISH->states.back();
			ISH->states.pop_back();
		}
	}
};

template <typename T>
struct TState : public State {
	typedef State* (T::*tfptr_t)(int, float*);
	T* obj;
	std::map<tfptr_t, std::array<int, 4>, std::less<tfptr_t const> > m;

	TState(T * obj_in) {
		ISH = 0;
		obj = obj_in;
	}
	TState(InputStateHierarchy * ISH_in, T * obj_in) {
		ISH = ISH_in;
		obj = obj_in;
	}
	~TState() {
		delete obj;
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
			bool check_for = (bool)it->second[2];
			int pass_dev = it->second[3];

			bool mask = (ISH->da_mask[dev]->data[ind] >> (bool)check_for) & 1;
			bool keyval = ISH->da[dev]->data[ind];
			if( !(mask || (keyval ^ check_for)) ) {
				State * n_state = (obj->*(it->first))(ISH->da[pass_dev]->vsize, ISH->da[pass_dev]->vals);
				if( (int)n_state >= -ISH->states.size() && (int)n_state < 0 ) {
					return ISH->popStates(-(int)n_state);
				} else if( n_state ) {
					n_state->setISH(ISH);
					ISH->pushState(n_state);
				}
			}
			ISH->da_mask[dev]->data[ind] |= 1 << (bool)check_for;
		}
	}
	virtual void Render() {
//Uncomment when Scene is implemented as a base class.
		obj->Render();
	}
};


#endif
