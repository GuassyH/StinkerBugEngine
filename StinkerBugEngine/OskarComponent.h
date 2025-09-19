#pragma once

#include "Component.h"

struct OskarComponent : public Component {
public:
	virtual void DrawOnInspector() override {
		if (ImGui::CollapsingHeader("Oskar Component")) {

		}
	}
};