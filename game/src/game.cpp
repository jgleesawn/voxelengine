#include "game.h"

Game::Game() {
//glewExperimental fixed segfault on glVertexAttribDivisor for GL2.0(on my laptop)
	glewExperimental = GL_TRUE;
	glewInit();
	if( GLEW_ARB_instanced_arrays )
		std::cout << "instanced arrays" << std::endl;
	else
		std::cout << "no instanced arrays" << std::endl;
		
	ren = new InstRenderer<ObjModel>();
	ren->Initialize();
	ren->setGLM(&glm);

	std::cerr << "renderer loaded." << std::endl;

	view = new Viewport(glm::vec4(0.0f, 0.0f, 20.0f, 0.0f));
	w.camera = w.addObject(view, view->getPosition());
	w.objects[w.camera]->rigidBody->setCollisionFlags(
		w.objects[w.camera]->rigidBody->getCollisionFlags() |
		btCollisionObject::CF_KINEMATIC_OBJECT);
	w.objects[w.camera]->rigidBody->setActivationState(DISABLE_DEACTIVATION);

	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_CULL_FACE);
	glDisable(GL_CULL_FACE);

for( int j=0; j<1; j++ ) {
	int instance_id = glm.LoadInst("res/nonTriangle/untitled.obj");

	for( int i=0; i<50; i++ ) {
//		glm::vec4 pos((float)rand()/RAND_MAX, (float)rand()/RAND_MAX, (float)rand()/RAND_MAX, (float)rand()/RAND_MAX );
		float x = i%3; x -= 1.5; x *= 5.0;
		float y = (i/9); y *= 5.0; y += 50.0f;
		float z = (i/3)%3; z -= 1.5; z *= 5.0;
		glm::vec4 pos(x,y,z, (float)rand()/RAND_MAX );
		pos.w = 0.0f;
		glm::quat q((float)rand()/RAND_MAX - .5, (float)rand()/RAND_MAX - .5, (float)rand()/RAND_MAX - .5, (float)rand()/RAND_MAX - .5 );
		q = glm::normalize(q);
		Renderable * ro = new Renderable(pos, instance_id, q, 0, 0, 10, new btBoxShape(btVector3(1.0, 1.0, 1.0)));
		ro->rigidBody->setRestitution(0.01f);
//		std::cerr << ro->getType() << " ";

		w.addObject(ro,pos);
		w.makeRenderable(ro->index);
	}
	
}
	w.terrain->setValues(&glm);
	w.terrain->GenerateTerrain();

	ISH.pushState(new TState<World>(&ISH, &w));
}
Game::~Game() {
	if( ren )
		delete ren;
}

void Game::Loop() {
//	std::cout << w.ot.UR[0] << std::endl;
//	interface.Loop(&w);
	ISH.update();
	ISH.processInputs();

//	w.Wiggle();
	w.update();

//	btTransform trans;
//	w.objects[1]->getWorldTransform(trans);
//	std::cout << (float)trans.getOrigin().x() << " " << (float)trans.getOrigin().y() << " " << (float)trans.getOrigin().z() << std::endl;


	std::vector<float> k_sqr;
	w.selection.clear();
//	w.octree.nearestKSearch(*w.cloud, w.camera, 15, w.selection, k_sqr);

	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

//	glm::vec4 * vpos = (glm::vec4 *)&(w.cloud->points[w.camera]);
	glm::vec4 vpos = w.objects[w.camera]->getPosition();
	ren->Use();
	ren->setCameraRotationPerspective( view->getRotMat(), view->perspectiveMatrix );
	ren->setCameraPos(vpos);

	glm::vec4 dir = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)*view->getRotMat();
//	printv(dir);
	w.terrain->Center( vpos, dir );
//	std::cout << "Current Density: " << plane((vpos)[0], (vpos)[1], (vpos)[2]) << std::endl;

	int id, ind;
	InstInfo<ObjModel> ii;

	std::map<int, std::vector<InstInfo<ObjModel> > > renderInfo;
	for( int i=0; i<w.renObjs.size(); i++ ) {
		w.renObjs[i]->addRenderInfo(renderInfo);
	}

	glm::vec4 llb(0.0f), urf(0.0f);
	double x,y,z;
//	w.octree.getBoundingBox(x, y, z, (double&)urf.x, (double&)urf.y, (double&)urf.z);
	llb.x = x; llb.y = y; llb.z = z;
//	float resolution = w.octree.getResolution();
	float resolution = 1.0f;

//llb is currently not actually relied on outside of being an argument.
	std::map<int, std::vector<InstInfo<ObjModel> > >::iterator it;
	for( it = renderInfo.begin(); it != renderInfo.end(); it++ ) {
		ren->RenderInst(*glm.gfxInst[it->first], it->second, llb, resolution);
	}


//Mouse over object selection based on btCollisionWorld rayTest
	renderInfo.clear();
	int xmi, ymi;
	SDL_GetMouseState(&xmi,&ymi);
	float xmf, ymf;
	xmf = -(float)xmi / 640.0f;
	ymf = -(float)ymi / 480.0f;
	xmf += 0.5f;
	ymf += 0.5f;
//	std::cerr << "x: " << xmf << " y: " << ymf << std::endl;
	glm::vec4 vstart, vend;
	btVector3 start, end;

	std::pair<glm::vec4, glm::vec4> p = view->getCloseFar(xmf, ymf);
	vstart = p.first;
	vend = p.second;
	start = *(btVector3 *)&(vstart);
	end = *(btVector3 *)&(vend);

	btCollisionWorld::AllHitsRayResultCallback crr_callback(start, end);
//	btKinematicClosestNotMeRayResultCallback * crr_callback = new btKinematicClosestNotMeRayResultCallback(w.objects[w.camera]->rigidBody);
	w.dynamicsWorld->rayTest(start, end, crr_callback);
	btRigidBody * rb;
	Object * obj;
	for( int i=0; i<crr_callback.m_collisionObjects.size(); i++ ) {
		rb = (btRigidBody *) crr_callback.m_collisionObjects[i];
		obj = (Object *)(rb->getMotionState());
		if( obj->getType() != 10 ) {
			continue;
		}
//		std::cerr << "Ind: " << obj->index << std::endl;
		((Renderable *)obj)->addRenderInfo(renderInfo);
	}
	llb = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
	for( it = renderInfo.begin(); it != renderInfo.end(); it++ ) {
		ren->WireframeInst(*glm.gfxInst[it->first], it->second, llb, resolution);
	}
	renderInfo.clear();

	renderInfo.clear();
	renderInfo = w.terrain->getRenderMap();
	for( it = renderInfo.begin(); it != renderInfo.end(); it++ ) {
//		ren->WireframeInst(*glm.gfxInst[it->first], it->second, llb, resolution);
		ren->RenderInst(*glm.gfxInst[it->first], it->second, llb, resolution);
	}

	renderInfo.clear();
	renderInfo = w.terrain->getDebugRenderMap();
	llb = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
	for( it = renderInfo.begin(); it != renderInfo.end(); it++ ) {
		ren->WireframeInst(*glm.gfxInst[it->first], it->second, llb, resolution);
	}

//Second Debug Rendering while color is passed by uniform.
//Usage coincides with note in terrain.h
	renderInfo.clear();
	renderInfo = w.terrain->getDebugRenderMap(true);
	llb = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	for( it = renderInfo.begin(); it != renderInfo.end(); it++ ) {
		ren->WireframeInst(*glm.gfxInst[it->first], it->second, llb, resolution);
	}


	ren->DebugGrid();

//FIX THIS, relying on camera being Object[0]
//	if( w.focus ) {
//		glm::vec4 * opos = (glm::vec4 *)&(w.cloud->points[w.objects[w.focus]->index]);
//		ren->Wireframe((Renderable *)w.objects[w.focus], *opos);
//	}
}




