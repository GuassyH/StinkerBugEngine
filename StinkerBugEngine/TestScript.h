#pragma once

#include "EntityBehaviour.h"

class TestScript : public EntityBehaviour {
public:
	virtual void Update() override {

		if (glfwGetKey(Display::getInstance().window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			if (!transform->HasComponent<RigidBody>()) {
				transform->AddComponent<RigidBody>();
			}
		}
	}
};
