#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>

#include <btBulletDynamicsCommon.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "util/glm_util.h"

class Object : public btDefaultMotionState {
public:
	int index;
//	glm::vec4 position;
	float pitch, yaw;
//	glm::quat orientation;
	float mass;
	btCollisionShape * shape;
	btVector3 inertia;
	btRigidBody* rigidBody;

	Object(glm::vec4, glm::quat or_in = glm::quat(), float p_in=0.0f, float y_in=0.0f, float m_in=10.0f, btCollisionShape * shape_in = new btSphereShape(1));
	~Object();

	void Move(glm::vec4 offset);// { position += offset; }
	void dQuat(const glm::quat &);

	glm::vec4 getPosition();

//	void rotParallel(float);
//	void rotPerpendicular(float);

//Uses angles not orientation variable
	glm::vec4 getForward();
	glm::vec4 getRight();
	glm::vec4 getUp();

	glm::mat4 getRotMat();
	glm::mat4 getQRotMat();
	void rotY(float);
	void rotX(float);
};

#endif
