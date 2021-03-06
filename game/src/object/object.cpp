#include "object.h"


Object::Object(glm::vec4 pos_in, glm::quat or_in, float p_in, float y_in, float m_in, btCollisionShape * shape_in) {
	type = 0;
	btTransform trans(*(btQuaternion *)&or_in, *(btVector3 *)&pos_in);
	setWorldTransform(trans);
	pitch = p_in;
	yaw = y_in;
	mass = m_in;
	shape = shape_in;
	shape->calculateLocalInertia(mass, inertia);
	btRigidBody::btRigidBodyConstructionInfo RigidBodyCI(mass, this, shape, inertia);

	rigidBody = new btRigidBody(RigidBodyCI);	
	
}

Object::~Object() {
	delete shape;
	delete rigidBody;
}

void Object::Move(glm::vec4 offset) {
	btTransform trans;
	getWorldTransform(trans);
	trans.setOrigin(trans.getOrigin() + *(btVector3 *)&offset);
	setWorldTransform(trans);
}

void Object::dQuat(const glm::quat & q) {
//	orientation *= q;
}

glm::vec4 Object::getPosition() {
	btTransform trans;
	getWorldTransform(trans);
	return *(glm::vec4 *)&trans.getOrigin();
}
/*
void Object::rotParallel(float rad) {
	glm::quat q;
	q.x = verticle.x*glm::sin(rad/2.0f);
	q.y = verticle.y*glm::sin(rad/2.0f);
	q.z = verticle.z*glm::sin(rad/2.0f);
	q.w = glm::cos(rad/2.0f);

	orientation = glm::rotate(q, orientation);
	std::cout << "Orientation: ";
	for( int i=0; i<3; i++ )
		std::cout << orientation[i] << " ";
	std::cout << std::endl;
}
*/
/*
void Object::rotPerpendicular(float rad) {
	glm::vec3 v = verticle;
	glm::vec3 ov = orientation;
	glm::vec3 c = glm::cross(v, ov);
	c = glm::normalize(c);
	glm::quat q = glm::quat(glm::cos(rad/2.0f), c.x*glm::sin(rad/2.0f), c.y*glm::sin(rad/2.0f), c.z*glm::sin(rad/2.0f));;

	orientation = glm::rotate(q, orientation);
}
*/
//Breaks regular object movement
//Fix it by reverting to 0,0,1,0
glm::vec4 Object::getForward() {
	return glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)*getRotMat();
}

glm::vec4 Object::getRight() {
	return glm::vec4(1.0f, 0.0f, 0.0f, 0.0f)*getRotMat();
}

glm::vec4 Object::getUp() {
	return glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)*getRotMat();
}

glm::mat4 Object::getRotMat() {
	return glm::eulerAngleXY(pitch, yaw);
}
glm::mat4 Object::getQRotMat() {
	btTransform trans;
	getWorldTransform(trans);
	btQuaternion q = trans.getRotation();
	return glm::toMat4(*(glm::quat *)&q);
}

void Object::rotY(float rad) {
	yaw += rad;
}

void Object::rotX(float rad) {
	pitch += rad;
}


