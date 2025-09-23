#ifndef COMPONENT_H
#define COMPONENT_H

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Entity.h"

class Component {
public:	
	Entity entity = 0;

	Component() = default;
	virtual ~Component() = default;
	virtual void DrawOnInspector() {}
	
};



#endif