#ifndef RIGIDBODY_COMPONENT_H
#define RIGIDBODY_COMPONENT_H

#include "glm/glm.hpp"

struct RigidBody {
	bool useGravity;
	bool isKinematic;

	glm::vec3 velocity;
};

inline RigidBody CreateRigidBody() {
	RigidBody rb;
	rb.useGravity = true;
	rb.isKinematic = false;
	rb.velocity = glm::vec3(0.0);
	return rb;
}

#endif