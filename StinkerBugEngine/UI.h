#ifndef UI_CLASS_H
#define UI_CLASS_H

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Display.h"

class CameraMovement;
class Scene;
struct Mesh;
struct Material;

class UI {
private:
	uint32_t selected_entity = 0;

	UI() = default;
	Display& display = Display::getInstance();

	unsigned int r_windowWidth;
	unsigned int r_windowHeight;
	bool opened = true;
	bool closed = false;
public:
	static UI& getInstance() { static UI instance; return instance; }

	void imgui_init();
	void imgui_render(CameraMovement& camera_move, Scene& scene, Mesh& cube, Material& mat);
	void imgui_shutdown();

	void Hierarchy(CameraMovement& camera_move, Scene& scene);
	void EntityInspector(CameraMovement& camera_move, Scene& scene, Mesh& cube, Material& mat);
};


#endif