#ifndef VIEWPORT_H
#define VIEWPORT_H

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <iostream>
#include <functional>

#include <utility>

#include "object.h"

class Viewport : public Object {
public:
	float cdist, fdist, aspect, fov;
	glm::mat4 perspectiveMatrix;

	Viewport(glm::vec4, glm::quat or_in = glm::quat(), float p_in=0.0f, float y_in=0.0f, float m_in=0.0f, btCollisionShape * shape_in = new btSphereShape(1), float cdist_in = 0.1f, float fdist_in = 100.0f, float aspect_in = 1.0f, float fov_in = glm::radians(45.0f));
	void Move(glm::vec4);

	void rotUp(){ rotX(-.1); }
	void rotDown(){ rotX(.1); }
	void rotLeft(){ rotY(-.1); }
	void rotRight(){ rotY(.1); }
//If you make void Forward() virtual it would fix camera movement
//if you revert the object.h void Forward();
	void Forward() { Move(glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)); }

	std::pair<glm::vec4, glm::vec4> getCloseFar(float, float);
//	glm::vec4 getClose(float, float);
//	glm::vec4 getFar(float, float);
};

#endif
