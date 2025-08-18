#pragma once

#include "EntityBehaviour.h"
#include "Entity.h"
#include "Display.h"

class SphereMove : public EntityBehaviour {
public:
	RigidBody* rb = nullptr;
	Display& display = Display::getInstance();
	DeltaTime& deltaTime = DeltaTime::getInstance();

	
	SphereMove() = default;

	void Start() override {
		rb = &entity->GetComponent<RigidBody>();
	}

	void Update() override {

		if (glfwGetKey(display.window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			rb->velocity += glm::vec3(5.0, 0.0, 0.0) * deltaTime.get();
		}
		if (glfwGetKey(display.window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			rb->velocity += glm::vec3(-5.0, 0.0, 0.0) * deltaTime.get();
		}
		if (glfwGetKey(display.window, GLFW_KEY_UP) == GLFW_PRESS) {
			rb->velocity += glm::vec3(0.0, 18.0, 0.0) * deltaTime.get();
		}

	}

};
