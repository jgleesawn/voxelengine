#ifndef DEVICE_H
#define DEVICE_H

#include <SDL2/SDL.h>
#include <string.h>

class Device {
public:
	std::vector<unsigned char>  data;
	float * vals;
	size_t vsize;
	virtual void update() = 0;
	void clear() {
		int dsize = data.size();
		data.resize(0);
		data.resize(dsize, (int)0);
	}
};

//Add proper interface between SDLMouse and window size instead of hardcoded ratios.
//Using size of 6 despite 5 buttons to let it align with SDL button values.
class SDLMouse : public Device {
public:
	SDLMouse() {
		vsize = 2;
		vals = new float[vsize];
		data.resize(6);
	}
	~SDLMouse() {
		delete vals;
	}
	virtual void update() {
		data.resize(0,0);
		data.resize(6,0);
		int x, y;
		unsigned int mstate = SDL_GetMouseState(&x, &y);
		vals[0] = (float)x/640.0f;
		vals[1] = (float)y/480.0f;
		vals[0] *= -1.0f; vals[0] += 0.5f;
		vals[1] *= -1.0f; vals[1] += 0.5f;

		for( int i=1; i<6; i++ )
			data[i] = mstate & SDL_BUTTON(i);
	}	
};

class SDLKeyboard : public Device {
public:
	SDLKeyboard() {
		vsize = 0;
		int klen;
		SDL_GetKeyboardState(&klen);
		data.resize(klen);
	}
	virtual void update() {
		int klen;
		const unsigned char * kstate = SDL_GetKeyboardState(&klen);
		data.resize(0,0);
		data.resize(klen,0);
		for( int i=0; i<data.size(); i++ )
			data[i] = kstate[i];
	}
};

#endif
