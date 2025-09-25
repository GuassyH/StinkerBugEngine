#ifndef LIGHT_COMPONENT_H
#define LIGHT_COMPONENT_H

#include "Component.h"

#include <iostream>
#include "glm/glm.hpp"

enum class LightTypes : uint8_t {
	Directional = 0,
	Spotlight = 1,
	Point = 2,
	Area = 3
};

class Light : public Component {
public:
	// Global
	LightTypes light_type = LightTypes::Spotlight;
	glm::vec4 color = glm::vec4(1.0f);
	glm::vec3 vec_direction;
	glm::mat4 light_VP = glm::mat4(1.0f);

	// Spotlight
	float distance;
	float radius_i;
	float radius_o;

	virtual void DrawOnInspector() override {
		if (ImGui::CollapsingHeader("Light")) {
			const char* items[] = { "Directional", "Spotlight", "Point", "Area" };
			int current_item = static_cast<int>(light_type);
			if (ImGui::Combo("Light Type", &current_item, items, IM_ARRAYSIZE(items))) {
				light_type = static_cast<LightTypes>(current_item);
			}
			ImGui::ColorEdit4("Color", glm::value_ptr(color));
	
			ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

			switch(light_type) {
			case LightTypes::Spotlight:
				ImGui::DragFloat("Distance", &distance, 0.1f, 0.0f);
				ImGui::DragFloat("Inner Radius", &radius_i, 0.1f, 0.0f);
				ImGui::DragFloat("Outer Radius", &radius_o, 0.1f, 0.0f);
			case LightTypes::Area:
				ImGui::DragFloat("Inner Radius", &radius_i, 0.1f, 0.0f);
				ImGui::DragFloat("Outer Radius", &radius_o, 0.1f, 0.0f);
			}
		}
	}
};


#endif