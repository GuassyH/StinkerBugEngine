#ifndef LIGHT_COMPONENT_H
#define LIGHT_COMPONENT_H

#include <iostream>
#include "glm/glm.hpp"

enum class LightTypes : uint8_t {
	Spotlight = 0,
	Directional = 1,
	Point = 2,
	Area = 3
};

struct Light {
	LightTypes light_type = LightTypes::Spotlight;
	glm::vec4 color = glm::vec4(1.0);
};


#endif