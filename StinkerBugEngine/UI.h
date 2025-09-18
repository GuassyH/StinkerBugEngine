#ifndef UI_CLASS_H
#define UI_CLASS_H

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Display.h"
#include "ConsoleCapture.h"
#include "Entity.h"

class CameraMovement;
class Scene;
struct Mesh;
struct Material;

class UI {
private:
	Entity selected_entity = 10;
	bool is_entity_selected = false;

	UI() = default;
	Display& display = Display::getInstance();

	unsigned int r_windowWidth;
	unsigned int r_windowHeight;
	bool opened = true;
	bool closed = false;
	ConsoleCapture consoleCapture;
public:
	static UI& getInstance() { static UI instance; return instance; }

	void imgui_init();
	void imgui_render(CameraMovement& camera_move, Scene& scene);
	void imgui_shutdown();

	void Hierarchy(Scene& scene);
	void EntityInspector(Scene& scene);
	void Console();
};


#endif