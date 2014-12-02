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

std::pair<glm::vec4, glm::vec4> Viewport::getCloseFar(float xmf, float ymf) {
	glm::quat qv, qr;
	float close_height, close_width;
	float cdist = 0.1f;
	float fdist = 100.0f;
	close_height = close_width = cdist*glm::tan(glm::radians(45.0f));
	float av = glm::atan(xmf * close_width / cdist);
	float ar = glm::atan(ymf * close_height / cdist);
	glm::vec4 vup = getUp();
	for( int i=0; i<4; i++ )
		qv[i] = vup[i] * glm::sin(av/2.0f);
	qv[3] = glm::cos(av/2.0f);
	glm::vec4 vright = getRight();
	for( int i=0; i<4; i++ )
		qr[i] = vright[i] * glm::sin(ar/2.0f);
	qr[3] = glm::cos(ar/2.0f);

	glm::vec4 forward = getForward();
	glm::vec3 f3(forward.x, forward.y, forward.z);
	f3 = glm::normalize(f3);
	glm::vec3 v3start = cdist * f3;
	v3start = glm::rotate(qv, v3start);
	v3start = glm::rotate(qr, v3start);

	glm::vec3 v3end = fdist * f3;
	v3end = glm::rotate(qv, v3end);
	v3end = glm::rotate(qr, v3end);

	glm::vec4 vstart(v3start, 0.0f);
	glm::vec4 vend(v3end, 0.0f);
	
	vstart = vstart + getPosition();
	vend = vend + getPosition();

	std::pair<glm::vec4, glm::vec4> p;
	p.first = vstart;
	p.second = vend;
	return p;
}
/*
glm::vec4 Viewport::getFar(float x, float y) {
}
*/
