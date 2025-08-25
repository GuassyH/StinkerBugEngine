#ifndef UI_CLASS_H
#define UI_CLASS_H

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Display.h"

class CameraMovement;
class Scene;

class UI {
private:
	UI() = default;
	Display& display = Display::getInstance();

	unsigned int r_windowWidth;
	unsigned int r_windowHeight;
	bool opened = true;
	bool closed = false;
public:
	static UI& getInstance() { static UI instance; return instance; }

	void imgui_init();
	void imgui_render(CameraMovement& camera_move, Scene& scene);
	void imgui_shutdown();
};


#endif