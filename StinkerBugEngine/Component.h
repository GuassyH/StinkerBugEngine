#ifndef ComponentCore_H
#define ComponentCore_H


#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Entity.h"

#include <memory>
#include "WeakPtrProxy.h"

class ECSystem;
class Transform;

class Component {
public:	
	Entity entity = 0;
	WeakPtrProxy<Transform> transform = nullptr;
	ECSystem* parent_ecs = nullptr;


	bool InspectorHasInit = false;
	Component() = default;
	virtual ~Component() = default;
	virtual void Init() {}
	virtual void DrawOnInspectorInit() {}
	virtual void DrawOnInspector() {}
	virtual void DrawGizmo() {}
};



#endif