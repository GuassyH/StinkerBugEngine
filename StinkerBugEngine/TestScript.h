#pragma once

#include "EntityBehaviour.h"

class TestScript : public EntityBehaviour {
public:
	virtual void Update() override {
		if (glfwGetKey(Display::getInstance().window, GLFW_KEY_1) == GLFW_PRESS) {
			std::cout << "First Script Activated" << std::endl;
		}
	}
};
