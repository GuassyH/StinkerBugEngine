#include <iostream>

#include "Display.h"
#include "Constants.h"
#include "Mesh.h"


int main(void) {

	Display& display = Display::getInstance();
	if (display.init(1920, 1080, "Stinker Bug Engine") == -1) { return -1; }

	Camera camera = Camera(display.windowWidth, display.windowHeight, glm::vec3(0.0));
	camera.transform.position = glm::vec3(0.5, 0.5, 2.0f);

	DeltaTime& deltaTime = DeltaTime::getInstance();


	Material mat;

	Mesh box = Mesh(Constants::Shapes::Cube(), mat);
	Mesh box2 = Mesh(Constants::Shapes::Cube(), mat);
	Mesh floor = Mesh(Constants::Shapes::Plane(), mat);
	Mesh backWall = Mesh(Constants::Shapes::Plane(), mat);
	Mesh globe = Mesh(Constants::Shapes::UVSphere(), mat);

	box2.transform.position = glm::vec3(0.5, 1, 0.5);
	box2.transform.rotation = glm::vec3(0, 45, 0);
	box2.transform.scale = glm::vec3(0.24);

	floor.transform.scale = glm::vec3(10);
	floor.transform.position = glm::vec3(-5, 0, -5);

	backWall.transform.scale = glm::vec3(10, 0.0, 0.25);
	backWall.transform.position = glm::vec3(-5, 0, -5);
	backWall.transform.rotation = glm::vec3(-90, -90, 0);

	globe.transform.position = glm::vec3(0, globe.transform.scale.y/2, 0);

	glm::vec3 velocity = glm::vec3(0);
	while (!glfwWindowShouldClose(display.window)) {
		display.BeginFrame();

		if (globe.transform.position.y <= floor.transform.position.y + globe.transform.scale.y / 2) {
			velocity.y = 3;
		}

		velocity.y -= 4.905 * deltaTime.get();

		globe.transform.position += velocity * deltaTime.get();

		camera.UpdateMatrix(75.0f, 0.1f, 1000.0f, display.windowWidth, display.windowHeight);
		camera.Input();

		// box.Draw(camera);
		// box2.Draw(camera);
		floor.Draw(camera);
		backWall.Draw(camera);
		globe.Draw(camera);

		display.EndFrame();
	}

	box.~Mesh();
	box2.~Mesh();
	floor.~Mesh();

	return 0;
}