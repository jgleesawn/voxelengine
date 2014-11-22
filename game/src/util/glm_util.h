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

bool lt(const glm::vec4 &, const glm::vec4 &);
bool gt(const glm::vec4 &, const glm::vec4 &);
bool lte(const glm::vec4 &, const glm::vec4 &);
bool gte(const glm::vec4 &, const glm::vec4 &);
bool eq(const glm::vec4 &, const glm::vec4 &);



#endif
