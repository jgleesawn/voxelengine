#ifndef DEVICE_H
#define DEVICE_H

#include <SDL2/SDL.h>
#include <string.h>

class Device {
public:
	std::vector<unsigned char>  data;
	int * vals;
	size_t vsize;
	virtual void update() = 0;
};

class SDLMouse : public Device {
public:
	SDLMouse() {
		vsize = 2;
		vals = new int[vsize];
		data.resize(5);
	}
	~SDLMouse() {
		delete vals;
	}
	virtual void update() {
		data.resize(0);
		data.resize(5);
		unsigned int mstate = SDL_GetMouseState(vals, &vals[1]);

		for( int i=0; i<5; i++ )
			data[i] = SDL_BUTTON(i+1);
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
		data.resize(0);
		data.resize(klen);
		for( int i=0; i<data.size(); i++ )
			data[i] = kstate[i];
	}
};

#endif
