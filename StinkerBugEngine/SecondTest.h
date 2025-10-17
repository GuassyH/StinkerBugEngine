#pragma once

#include "EntityBehaviour.h"

class SecondTest : public EntityBehaviour {
public:
	virtual void Update() override {
		if (glfwGetKey(Display::getInstance().window, GLFW_KEY_2) == GLFW_PRESS) {
			std::cout << "Second Script Activated" << std::endl;
		}
	}
};
