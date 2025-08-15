#include <iostream>

#include "Display.h"
#include "Constants.h"
#include "SceneManager.h"
#include "Mesh.h"
#include "FullScreenPass.h"

int main(void) {
	Display& display = Display::getInstance();
	if (display.init(1920, 1080, "Stinker Bug Engine") == -1) { return -1; }

	Camera camera = Camera(display.windowWidth, display.windowHeight, glm::vec3(0.0));
	camera.transform.position = glm::vec3(0.5, 0.5, 2.0f);

	DeltaTime& deltaTime = DeltaTime::getInstance();
	SceneManager& sceneManager = SceneManager::getInstance();
	Scene sceneOne;

	sceneManager.LoadScene(sceneOne);

	Shader skybox_shader("skybox.vert", "skybox.frag");

	Material skybox_mat(skybox_shader);
	Material material;
	Material red; red.Color = Constants::Colors::Red;

	Mesh floor = Mesh(Constants::Shapes::Plane(), material);
	Mesh backWall = Mesh(Constants::Shapes::Plane(), material);
	Mesh globe = Mesh(Constants::Shapes::UVSphere(), material);
	Mesh box = Mesh(Constants::Shapes::Cube(), red);

	FullScreenPass skybox_pass = FullScreenPass(camera, skybox_mat);
	

	floor.transform.scale = glm::vec3(10);
	floor.transform.position = glm::vec3(-5, 0, -5);

	backWall.transform.scale = glm::vec3(10, 0.0, 0.25);
	backWall.transform.position = glm::vec3(-5, 0, -5);
	backWall.transform.rotation = glm::vec3(-90, -90, 0);

	box.transform.position = glm::vec3(-3, 0.0, 0.0);
	globe.transform.position = glm::vec3(0, globe.transform.scale.y / 2, 0);


	glm::vec3 velocity = glm::vec3(0);
	while (!glfwWindowShouldClose(display.window)) {
		display.BeginFrame();
		skybox_pass.Draw(camera);

		if (globe.transform.position.y <= floor.transform.position.y + globe.transform.scale.y / 2) {
			velocity.y = 3;
		}
		velocity.y -= 4.905 * deltaTime.get();


		globe.transform.position += velocity * deltaTime.get();

		camera.UpdateMatrix(75.0f, 0.1f, 1000.0f, display.windowWidth, display.windowHeight);
		camera.Input();



		sceneManager.DrawScene(camera);


		display.EndFrame();
	}

	

	return 0;
}