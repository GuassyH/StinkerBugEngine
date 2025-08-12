
#include <iostream>
#include "Display.h"

#include "CommonShapes.h"
#include "Mesh.h"


int main(void) {

	Display& display = Display::getInstance();
	if (display.init(1920, 1080, "Stinker Bug Engine") == -1) { return -1; }

	Camera camera = Camera(display.windowWidth, display.windowHeight, glm::vec3(0.0));
	camera.transform.position = glm::vec3(0.0, 0.0, 1.0f);

	Shader shader("default.vert", "default.frag");
	Mesh tri = Mesh(Triangle::vertices, Triangle::indices);

	while (!glfwWindowShouldClose(display.window)) {
		display.BeginFrame();

		camera.UpdateMatrix(60.0f, 0.1f, 1000.0f, display.windowWidth, display.windowHeight);
		camera.Input();
		tri.Draw(shader, camera);

		display.EndFrame();
	}


	return 0;
}