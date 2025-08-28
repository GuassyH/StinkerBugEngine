#ifndef RIGIDBODY_COMPONENT_H
#define RIGIDBODY_COMPONENT_H

#include "Component.h"
#include "glm/glm.hpp"

struct RigidBody : public Component {
public:
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
		drag(0.4f),
		angular_drag(0.05f),
		bounciness(1.0f)
	{}

	virtual void DrawInInspector() override {
		if (ImGui::CollapsingHeader("Rigid Body")) {
			ImGui::DragFloat("Mass", &mass, 0.1f, 0.0f, 100.0f);
			ImGui::DragFloat("Bounciness", &bounciness, 0.1f, 0.0f, 1);
			ImGui::DragFloat("Drag", &drag, 0.1f, 0.0f, 1);
		}
	}
};

#endif