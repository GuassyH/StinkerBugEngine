#ifndef COMPONENT_EXPANDED_H
#define COMPONENT_EXPANDED_H

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Entity.h"

#include <memory>

class ComponentCore {
public:
	Entity entity = 0;

	ComponentCore() = default;
	virtual ~ComponentCore() = default;
	virtual void DrawOnInspector() {}
	virtual void DrawGizmo() {}
};


#endif