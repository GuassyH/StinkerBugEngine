#ifndef UI_CLASS_H
#define UI_CLASS_H

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Display.h"
#include "Entity.h"

#include "TopBarWindow.h"
#include "ConsoleWindow.h"

#include "HierarchyWindow.h"
#include "InspectorWindow.h"

#include "SceneViewWindow.h"
#include "GameWindow.h"

#include "ECSystem.h"

class Scene;
struct Mesh;
struct Material;

class UI {
private:
	Entity selected_entity = 10;
	bool is_entity_selected = false;

	UI() = default;
	Display& display = Display::getInstance();

	TopBarWindow topBarWindow;
	ConsoleWindow consoleWindow;
	HierarchyWindow hierarchyWindow;
	InspectorWindow inspectorWindow;
	SceneViewWindow sceneViewWindow;
	GameWindow gameWindow;
public:
	static UI& getInstance() { static UI instance; return instance; }

	ECSystem Editor_ECS;

	void imgui_init();
	void imgui_render(Scene& scene);
	void imgui_shutdown();

};


#endif