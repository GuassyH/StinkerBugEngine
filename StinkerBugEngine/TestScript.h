#pragma once

#include "EntityBehaviour.h"

class TestScript : public EntityBehaviour {
public:
	bool firstclick = true;
	virtual void Update() override {
		if (glfwGetKey(Display::getInstance().window, GLFW_KEY_1) == GLFW_PRESS) {
			if (!firstclick) {
				return;
			}
			firstclick = false;

			// it NEEDS to be an object, NOT a reference or pointer
			EntityObject newEnt = CreateEntity("Test Script Entity");
			newEnt.transform->AddComponent<MeshRenderer>(new Model(Constants::Shapes::Cube()), new Material(MaterialFlags_Lit | MaterialFlags_Shadow));
		}
		else {
			firstclick = true;
		}
	}
};
