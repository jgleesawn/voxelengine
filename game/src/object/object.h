#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

class Object {
public:
	int index;
	glm::vec4 position;
	float pitch, yaw;
	glm::quat orientation;

	Object(glm::vec4, glm::quat or_in = glm::quat(), float p_in=0.0f, float y_in=0.0f);

	void Move(glm::vec4 offset) { position += offset; }
	void dQuat(const glm::quat &);

//	void rotParallel(float);
//	void rotPerpendicular(float);

//Uses angles not orientation variable
	glm::vec4 getForward();
	glm::vec4 getRight();
	glm::vec4 getUp();

	glm::mat4 getRotMat();
	void rotY(float);
	void rotX(float);
};

void printv(const glm::vec3 &);
void printv(const glm::vec4 &);
void printq(const glm::quat &);
void printm(const glm::mat4 &);

#endif
