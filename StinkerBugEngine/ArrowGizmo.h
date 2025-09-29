#pragma once

#include "EntityBehaviour.h"

class ArrowGizmo : public EntityBehaviour {
public:
	void Update() override {
		glm::vec3 direction = transform->DegToRad();
	}
};