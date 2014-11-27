#ifndef VIEWPORT_H
#define VIEWPORT_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <iostream>
#include <functional>

#include "object.h"

class Viewport : public Object {
public:
	glm::mat4 perspectiveMatrix;

	Viewport(glm::vec4, glm::quat or_in = glm::quat(), float p_in=0.0f, float y_in=0.0f, float m_in=0.0f, btCollisionShape * shape_in = new btSphereShape(1), glm::mat4 perspec_in = glm::perspective(glm::radians(45.0f), 1.0f, .1f, 100.0f));
	void Move(glm::vec4);

	void rotUp(){ rotX(-.1); }
	void rotDown(){ rotX(.1); }
	void rotLeft(){ rotY(-.1); }
	void rotRight(){ rotY(.1); }
	void Forward() { Move(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)); }
};

#endif
