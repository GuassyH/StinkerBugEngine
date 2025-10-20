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

struct alignas(16) LightBufferObject {
	int type;
	int pad0;
	float radius_i;
	float radius_o;
	float radius;
	float intensity;
	float angle;
	int pad2; // pad to 32 byte
	glm::vec3 pos;
	float pad3;
	glm::vec3 dir;
	float pad4;
	glm::vec4 color;
};

class Light : public Component {
public:
	// Global
	LightTypes light_type = LightTypes::Spotlight;
	glm::mat4 light_VP = glm::mat4(1.0f);
	glm::vec4 color = glm::vec4(1.0f);
	float intensity = 1.0f;

	float radius = 1.0f;
	
	// Directional
	float ambient = 0.5f;

	// Spotlight
	float angle = 30.0f;

	// Point light
	bool cast_shadows = true;
	float radius_i = 0.5f;
	float radius_o = 1.0f;


	virtual void DrawOnInspector() override {
		if (ImGui::CollapsingHeader("Light")) {
			const char* items[] = { "Directional", "Spot", "Point", "Area" };
			int current_item = static_cast<int>(light_type);
			if (ImGui::Combo("Light Type", &current_item, items, IM_ARRAYSIZE(items))) {
				light_type = static_cast<LightTypes>(current_item);
			}
			ImGui::ColorEdit4("Color", glm::value_ptr(color));
	
			ImGui::Spacing(); ImGui::Spacing(); ImGui::Spacing();

			switch(light_type) {
			case LightTypes::Directional:
				ImGui::DragFloat("Ambient", &ambient, 0.1f, 0.0f, 1.0f);
				break;
			case LightTypes::Spotlight:
				ImGui::DragFloat("Range", &radius, 0.1f, 0.0f);
				ImGui::DragFloat("Angle", &angle, 0.1f, 0.0f);
				ImGui::DragFloat("Intensity", &intensity, 0.1f, 0.0f);
				break;
			case LightTypes::Point:
				ImGui::DragFloat("Inner Radius", &radius_i, 0.1f, 0.0f);
				ImGui::DragFloat("Outer Radius", &radius_o, 0.1f, 0.0f);
				ImGui::DragFloat("Intensity", &intensity, 0.1f, 0.0f);
				break;
			}
		}
	}
};


#endif