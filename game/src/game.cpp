#include "game.h"

Game::Game() {
//glewExperimental fixed segfault on glVertexAttribDivisor for GL2.0(on my laptop)
	glewExperimental = GL_TRUE;
	glewInit();
	if( GLEW_ARB_instanced_arrays )
		std::cout << "instanced arrays" << std::endl;
	else
		std::cout << "no instanced arrays" << std::endl;
		
	ren = new InstRenderer();
	ren->Initialize();
	ren->setGLM(&glm);

	std::cerr << "renderer loaded." << std::endl;

	view = new Viewport(glm::vec4(0.0f, 0.0f, 20.0f, 0.0f));
	w.camera = w.addObject(view, view->position);

	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);

for( int j=0; j<1; j++ ) {
	instance_ids.push_back(glm.LoadInst("res/nonTriangle/untitled.obj"));

	for( int i=0; i<50; i++ ) {
		glm::vec4 pos((float)rand()/RAND_MAX, (float)rand()/RAND_MAX, (float)rand()/RAND_MAX, (float)rand()/RAND_MAX );
		pos *= 200.0f;
		pos -= 100.0f;
//		pos.x = 0.0f;
//		pos.y = 0.0f;
//		pos.z = -0.5f;
		pos.w = 0.0f;
		glm::quat q((float)rand()/RAND_MAX - .5, (float)rand()/RAND_MAX - .5, (float)rand()/RAND_MAX - .5, (float)rand()/RAND_MAX - .5 );
		q = glm::normalize(q);
		Renderable * ro = new Renderable(pos, instance_ids.back(), q);

		w.addObject(ro,pos);
		w.makeRenderable(ro->index);
	}
}
	w.terrain.glm = &glm;
	w.terrain.chunk_size = 3.1415;
	w.terrain.pos = w.objects[w.camera]->position;
	w.terrain.pos.y -= .5f;
	w.terrain.GenerateTerrain();

//	interface.m[&World::MoveFocusForward] = SDL_SCANCODE_W;
	interface.m[&World::MoveFocusBack] = SDL_SCANCODE_W;
	interface.m[&World::MoveFocusLeft] = SDL_SCANCODE_A;
	interface.m[&World::MoveFocusRight] = SDL_SCANCODE_D;
//	interface.m[&World::MoveFocusBack] = SDL_SCANCODE_S;
	interface.m[&World::MoveFocusForward] = SDL_SCANCODE_S;

//	interface.m[&World::RotFocusRight] = SDL_SCANCODE_RIGHT;
//	interface.m[&World::RotFocusLeft] = SDL_SCANCODE_LEFT;
	interface.m[&World::RotFocusLeft] = SDL_SCANCODE_RIGHT;
	interface.m[&World::RotFocusRight] = SDL_SCANCODE_LEFT;
	interface.m[&World::RotFocusUp] = SDL_SCANCODE_UP;
	interface.m[&World::RotFocusDown] = SDL_SCANCODE_DOWN;

	interface.m[&World::focusCamera] = SDL_SCANCODE_SPACE;
	interface.m[&World::focusNext] = SDL_SCANCODE_TAB;
}

Game::~Game() {
	if( ren )
		delete ren;
}

void Game::addInput(const void * state, int count, int stepsize) {
	interface.state = (Uint8 *)state;
	interface.count = count;

//	input in = {state, count, stepsize};
//	inputs.push_back(in);

}
void Game::Loop() {
//	std::cout << w.ot.UR[0] << std::endl;
	interface.Loop(&w);

//	w.Wiggle();
//	w.update();


	std::vector<float> k_sqr;
	w.selection.clear();
	w.octree.nearestKSearch(*w.cloud, w.camera, 15, w.selection, k_sqr);

	
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glm::vec4 * vpos = (glm::vec4 *)&(w.cloud->points[w.camera]);
	ren->Use();
	ren->setCameraRotationPerspective( view->getRotMat(), view->perspectiveMatrix );
	ren->setCameraPos(*vpos);

	int id, ind;
	InstInfo ii;

	std::map<int, std::vector<InstInfo> > renderInfo;
	for( int i=0; i<w.renObjs.size(); i++ ) {
		id = w.renObjs[i]->instance_id;
		ind = w.renObjs[i]->index;
		*((glm::vec4 *) &ii.position) = *((glm::vec4 *) &w.cloud->points[ind]);
//		w.octree.genKey(w.cloud->points[ind], *((pcl::octree::OctreeKey*) ((uint32_t *)ii.position)));
		ii.depthMask_in = 1;
		renderInfo[id].push_back(ii);
	}

	glm::vec4 llb(0.0f), urf(0.0f);
	double x,y,z;
	w.octree.getBoundingBox(x, y, z, (double&)urf.x, (double&)urf.y, (double&)urf.z);
	llb.x = x; llb.y = y; llb.z = z;
	float resolution = w.octree.getResolution();

	std::map<int, std::vector<InstInfo> >::iterator it;
	for( it = renderInfo.begin(); it != renderInfo.end(); it++ ) {
		ren->RenderInst(*glm.gfxInst[it->first], it->second, llb, resolution);
	}

	renderInfo.clear();
//Starts at 1 because the closest to the camera is the camera itself. //assumption
	for( int i=1; i<w.selection.size(); i++ ) {
		id = ((Renderable *)w.objects[w.selection[i]])->instance_id;
		ind = w.objects[w.selection[i]]->index;
		*((glm::vec4 *) &ii.position) = *((glm::vec4 *) &w.cloud->points[ind]);
//		w.octree.genKey(w.cloud->points[ind], *((pcl::octree::OctreeKey*) ((uint32_t *)ii.position)));
		ii.depthMask_in = 1;
		renderInfo[id].push_back(ii);
	}

	for( it = renderInfo.begin(); it != renderInfo.end(); it++ ) {
		ren->WireframeInst(*glm.gfxInst[it->first], it->second, llb, resolution);
	}


	renderInfo.clear();
	for( int i=0; i<terrain_size; i++ ) {
		for( int j=0; j<terrain_size; j++ ) {
			for( int k=0; k<terrain_size; k++ ) {
				glm::vec4 tpos = w.terrain.pos;
				id = w.terrain.space[i][j][k]->instance_id;
				ii.position[0] = w.terrain.pos.x + w.terrain.chunk_size*(float)j;
				ii.position[1] = w.terrain.pos.y + w.terrain.chunk_size*(float)k;
				ii.position[2] = w.terrain.pos.z + w.terrain.chunk_size*(float)i;
/*
				ii.position[0] = w.terrain.space[i][j][k]->position.x;
				ii.position[1] = w.terrain.space[i][j][k]->position.y;
				ii.position[2] = w.terrain.space[i][j][k]->position.z;
*/
				ii.depthMask_in = 1.0f;
				renderInfo[id].push_back(ii);
			}
		}
	}	
	for( it = renderInfo.begin(); it != renderInfo.end(); it++ ) {
//		ren->WireframeInst(*glm.gfxInst[it->first], it->second, llb, resolution);
		ren->RenderInst(*glm.gfxInst[it->first], it->second, llb, resolution);
	}

//FIX THIS, relying on camera being Object[0]
//	if( w.focus ) {
//		glm::vec4 * opos = (glm::vec4 *)&(w.cloud->points[w.objects[w.focus]->index]);
//		ren->Wireframe((Renderable *)w.objects[w.focus], *opos);
//	}
}




