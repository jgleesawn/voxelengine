#include "viewport.h"

Viewport::Viewport(glm::vec4 pos_in, glm::quat or_in, float p_in, float y_in, float m_in, btCollisionShape * shape_in, glm::mat4 perspec_in) : Object(pos_in, or_in, p_in, y_in, m_in, shape_in) {
	type = 1;
	perspectiveMatrix = perspec_in;
}

void Viewport::Move(glm::vec4 offset) {
	btTransform trans;
	getWorldTransform(trans);
	trans.setOrigin(trans.getOrigin() - *(btVector3 *)&offset);
	setWorldTransform(trans);
//	position -= offset*getRotMat();
}


