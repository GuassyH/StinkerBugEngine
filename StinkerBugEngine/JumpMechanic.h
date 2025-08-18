#pragma once
#include "EntityBehaviour.h"
#include "Display.h"
#include "Entity.h"

class JumpMechanic : public EntityBehaviour {
public:
	RigidBody* rb = nullptr;

	Display& display = Display::getInstance();
	DeltaTime& deltaTime = DeltaTime::getInstance();

	JumpMechanic() = default;

	void Start() override {
		rb = &entity->GetComponent<RigidBody>();
	}

	bool pressed = false;
	void Update() override {
		if (glfwGetKey(display.window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			if (!pressed) {
				pressed = true;
				rb->velocity = glm::vec3(0.0, 20, 0.0);
			}
		}
		else {
			pressed = false;
		}
	}
};