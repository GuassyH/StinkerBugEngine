#ifndef COMPONENT_H
#define COMPONENT_H

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

class Component {
public:	
	Component() = default;
	virtual ~Component() = default;
	virtual void DrawInInspector() {}
	
};



#endif