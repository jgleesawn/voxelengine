#include "renderable.h"

Renderable::Renderable(glm::vec4 pos_in, int inst_id, glm::quat or_in, float p_in, float y_in, float m_in, btCollisionShape * shape_in ) : instance_id(inst_id), Object(pos_in, or_in, p_in, y_in, m_in, shape_in) { 
type = 10;
}

void Renderable::addRenderInfo(std::map<int, std::vector<InstInfo<ObjModel> > > & renderInfo) {
	InstInfo<ObjModel> ii;
	*((glm::vec4 *) &ii.position) = getPosition();
	ii.depthMask_in = 1;
	*(glm::mat4 *) &ii.rotMat = glm::transpose(getQRotMat());
	renderInfo[instance_id].push_back(ii);
}
