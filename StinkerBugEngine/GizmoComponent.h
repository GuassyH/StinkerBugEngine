#ifndef GIZMO_COMPONENT_H
#define GIZMO_COMPONENT_H

#include "Component.h"
#include "glm/glm.hpp"

struct GizmoComponent : public Component {
public:
	bool isHovered = false;
	bool interactable = true;
	glm::vec4 reg_color;
	glm::vec4 hover_color = glm::vec4(1.0f, 1.0f, 0.6f, 1.0f);
};

#endif