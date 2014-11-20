#ifndef GLM_UTIL_H
#define GLM_UTIL_H

#include <iostream>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

void printv(const glm::vec3 &);
void printv(const glm::vec4 &);
void printq(const glm::quat &);
void printm(const glm::mat4 &);

float sumv(const glm::vec4 &);


#endif
