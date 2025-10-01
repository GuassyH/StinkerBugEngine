#pragma once

#include "EntityBehaviour.h"
#include "Display.h"

class ArrowGizmo : public EntityBehaviour {
public:
	void Update() override {
		glm::vec3 direction = transform->DegToVec();
		Display& display = Display::getInstance();
	}
};