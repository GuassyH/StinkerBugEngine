#ifndef TRANSFORM_COMPONENT_H
#define TRANSFORM_COMPONENT_H

#include "glm/glm.hpp"

struct Transform {
	glm::vec3 position = glm::vec3(0.0);
	glm::vec3 rotation = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 scale = glm::vec3(1.0);
};


#endif