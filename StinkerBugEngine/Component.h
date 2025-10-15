#ifndef ComponentCore_H
#define ComponentCore_H


#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Entity.h"

#include <memory>

class Transform;

class Component {
public:	
	Entity entity = 0;
	std::shared_ptr<Transform> transform = nullptr;

	Component() = default;
	virtual ~Component() = default;
	virtual void DrawOnInspector() {}
	virtual void DrawGizmo() {}
};



#endif