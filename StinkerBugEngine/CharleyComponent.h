#pragma once


#include "Component.h"

struct CharleyComponent : public Component {
public:
	// Draw Charley bear, and her beautiful face, and eyes, and hands, and her beautiful voice

	virtual void DrawOnInspector() override {
		if (ImGui::CollapsingHeader("CharleyComponent")) {

		}
	}

};