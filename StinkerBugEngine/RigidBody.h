#ifndef RIGIDBODY_COMPONENT_H
#define RIGIDBODY_COMPONENT_H

#include "glm/glm.hpp"

struct RigidBody {
	bool useGravity;
	bool isKinematic;

	float mass;
	float drag;
	float angular_drag;

	float bounciness;

	glm::vec3 velocity;
	glm::vec3 angular_velocity;
	glm::vec3 centre_of_mass;

	RigidBody()
		: useGravity(true),
		isKinematic(false),
		velocity(glm::vec3(0.0f)),
		angular_velocity(glm::vec3(0.0)),
		centre_of_mass(glm::vec3(0.0)),
		mass(1.0f),
		drag(0.1f),
		angular_drag(0.05f),
		bounciness(1.0f)
	{}
};

#endif