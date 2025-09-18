#ifndef LIGHT_COMPONENT_H
#define LIGHT_COMPONENT_H

#include "Component.h"

#include <iostream>
#include "glm/glm.hpp"

enum class LightTypes : uint8_t {
	Spotlight = 0,
	Directional = 1,
	Point = 2,
	Area = 3
};

struct Light : public Component {
public:
	LightTypes light_type = LightTypes::Spotlight;
	glm::vec4 color = glm::vec4(1.0);

	virtual void DrawOnInspector() override {
		if (ImGui::CollapsingHeader("Light NOT WORKING")) {
			const char* items[] = { "Spotlight", "Directional", "Point", "Area" };
			int current_item = static_cast<int>(light_type);
			if (ImGui::Combo("Light Type", &current_item, items, IM_ARRAYSIZE(items))) {
				light_type = static_cast<LightTypes>(current_item);
			}
			ImGui::ColorEdit4("Color", glm::value_ptr(color));
		}
	}
};


#endif