#pragma once

#include "Component.h"

struct TestComponent : public Component {
	virtual void DrawOnInspector() override {
		if (ImGui::CollapsingHeader("Test Component")) {
			ImGui::Text("This is a TEST component");
			ImGui::Button("Test Button");
			ImGui::SameLine();
			ImGui::Button("Test Button Same Line");
		}
	}
};