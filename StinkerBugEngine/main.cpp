#include <iostream>

#include "Display.h"
#include "CommonShapes.h"
#include "Mesh.h"


int main(void) {

	Display& display = Display::getInstance();
	if (display.init(1920, 1080, "Stinker Bug Engine") == -1) { return -1; }

	Camera camera = Camera(display.windowWidth, display.windowHeight, glm::vec3(0.0));
	camera.transform.position = glm::vec3(0.5, 0.5, 2.0f);

	Shader shader("default.vert", "default.frag");
	Mesh box = Mesh(Shapes::Cube());
	Mesh box2 = Mesh(Shapes::Cube());
	Mesh floor = Mesh(Shapes::Plane());


	box2.transform.position = glm::vec3(0, 1, 0);
	box2.transform.rotation = glm::vec3(0, 45, 0);
	box2.transform.scale = glm::vec3(0.24);

	floor.transform.scale = glm::vec3(10);
	floor.transform.position = glm::vec3(-5, 0, -5);

	while (!glfwWindowShouldClose(display.window)) {
		display.BeginFrame();

		camera.UpdateMatrix(75.0f, 0.1f, 1000.0f, display.windowWidth, display.windowHeight);
		camera.Input();

		box.Draw(shader, camera);
		box2.Draw(shader, camera);
		floor.Draw(shader, camera);

		display.EndFrame();
	}

	box.~Mesh();
	box2.~Mesh();
	floor.~Mesh();

	return 0;
}