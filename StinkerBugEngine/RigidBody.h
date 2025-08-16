#ifndef RIGIDBODY_COMPONENT_H
#define RIGIDBODY_COMPONENT_H

#include "glm/glm.hpp"

struct RigidBody {
	bool useGravity;
	bool isKinematic;

	glm::vec3 velocity;

	RigidBody() : useGravity(true), isKinematic(false), velocity(glm::vec3(0)) {}
};

#endif