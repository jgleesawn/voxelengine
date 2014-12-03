#ifndef SCENE_H
#define SCENE_H

#include <glm/glm.hpp>
#include <map>
#include <vector>
#include "glmanager/instrenderer.h"
#include "glmanager/glmanager.h"

//T is type of Instance Information
template < typename T>
struct Scene {
	InstRenderer<T> * ren;
	GLmanager<T> * glm;
	std::map<int, std::vector<InstInfo<T> > > renderInfo;

	Scene() { ren = 0; glm = 0; }
	~Scene() { }
	void setGLM(GLmanager<T> * glm_in) { glm = glm_in; }
	void setRen(InstRenderer<T> * ren_in) { ren = ren_in; }
	virtual void ClearScene() { renderInfo.clear(); }
	virtual void LoadScene() { }
	virtual void Render() {
		float resolution(1.0f);
		glm::vec4 llb(0.0f);
		typename std::map<int, std::vector<InstInfo<T> > >::iterator it;
		for( it = renderInfo.begin(); it != renderInfo.end(); it++ ) {
			ren->RenderInst(*glm->gfxInst[it->first], it->second, llb, 1.0f);
		}
	}
	virtual void DebugRender() {
		float resolution(1.0f);
		glm::vec4 llb(0.0f, 1.0f, 1.0f, 1.0f);
		typename std::map<int, std::vector<InstInfo<T> > >::iterator it;
		for( it = renderInfo.begin(); it != renderInfo.end(); it++ ) {
			ren->WireframeInst(*glm->gfxInst[it->first], it->second, llb, 1.0f);
		}
	}
};

#endif
