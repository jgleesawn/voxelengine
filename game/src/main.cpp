#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <SDL2/SDL_opengl.h>
//#include <GL/glu.h>

#include <stdio.h>
#include <iostream>

#include <glm/glm.hpp>

#include "util/cleanup.h"
#include "util/sdlutil.h"

#include "game.h"

int main( int argc, char* args[] ) {
	SDL_Window* window = NULL;

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		logSDLError(std::cout, "SDL_Init");
		return 1;
	}

//	window = SDL_CreateWindow( "SDL Game", 100, 100, 640, 480, SDL_WINDOW_SHOWN );
	window = SDL_CreateWindow( "SDL Game", 100, 100, 640, 640, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
	if( window == nullptr ) {
		logSDLError(std::cout, "CreateWindow");
		SDL_Quit();
		return 1;
	}

	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
//	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

	SDL_GLContext glcontext = SDL_GL_CreateContext(window);

	int kbsize;
	const Uint8 * kb = SDL_GetKeyboardState(&kbsize);

	std::cerr << "pregame" << std::endl;	
	Game game;
	std::cerr << "preinput" << std::endl;
	game.addInput((void*)kb, kbsize, sizeof(Uint8));

	bool quit = false;
	SDL_Event event;

	while( !quit ) {

		while( SDL_PollEvent( &event ) != 0 ) {
			switch( event.type ) {
			case SDL_QUIT:
				quit = true;
				break;
			}
		}
		std::cerr << " Loop ";
		game.Loop();

		SDL_GL_SwapWindow(window);

//		SDL_Delay( 10 );
	}


	cleanup(&glcontext,window);
	SDL_Quit();

	return 0;
}


