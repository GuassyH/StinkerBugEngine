#pragma once

#include "EntityBehavour.h"

#include "Display.h"

class SphereMove : public EntityBehaviour {
public:
	Transform& transform;
	RigidBody& rb;
	Display& display = Display::getInstance();

	DeltaTime& deltaTime = DeltaTime::getInstance();
	
	SphereMove() = default;
	SphereMove(Transform& t, RigidBody& rb) : transform(t), rb(rb) {}


	void Update() override {

		if (glfwGetKey(display.window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			rb.velocity += glm::vec3(5.0, 0.0, 0.0) * deltaTime.get();
		}
		if (glfwGetKey(display.window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			rb.velocity += glm::vec3(-5.0, 0.0, 0.0) * deltaTime.get();
		}
		if (glfwGetKey(display.window, GLFW_KEY_UP) == GLFW_PRESS) {
			rb.velocity += glm::vec3(0.0, 18.0, 0.0) * deltaTime.get();
		}

	}

};
