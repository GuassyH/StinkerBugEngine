#include <iostream>

#include "Display.h"
#include "Constants.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Mesh.h"
#include "FullScreenPass.h"

#include "Collider.h"
#include "SphereCollider.h"


int main(void) {
	Display& display = Display::getInstance();
	if (display.init(1920, 1080, "Stinker Bug Engine") == -1) { return -1; }

	Camera camera = Camera(display.windowWidth, display.windowHeight, glm::vec3(0.0));
	camera.transform.position = glm::vec3(0.5, 0.5, 2.0f);

	DeltaTime& deltaTime = DeltaTime::getInstance();
	SceneManager& sceneManager = SceneManager::getInstance();
	Scene scene;

	sceneManager.SetActiveScene(scene);

	Shader skybox_shader("skybox.vert", "skybox.frag");

	Material skybox_mat(skybox_shader);
	Material material;
	Material red; red.Color = Constants::Colors::Red;

	

	Mesh floor = Mesh(Constants::Shapes::Plane());
	Mesh cube = Mesh(Constants::Shapes::Cube());

	FullScreenPass skybox_pass = FullScreenPass(camera, skybox_mat);

	EntityID e_floor = scene.CreateEntity();
	scene.transforms[e_floor] = { glm::vec3(0.0), glm::vec3(0.0), glm::vec3(4.0) };
	scene.meshRenderers[e_floor] = CreateMeshRenderer(floor, material);

	EntityID e_cube = scene.CreateEntity();
	scene.transforms[e_cube] = { glm::vec3(0.0), glm::vec3(0.0), glm::vec3(1.0) };
	scene.meshRenderers[e_cube] = CreateMeshRenderer(cube, red);

	while (!glfwWindowShouldClose(display.window)) {
		display.BeginFrame();
		skybox_pass.Draw(camera);


		camera.UpdateMatrix(75.0f, 0.1f, 1000.0f, display.windowWidth, display.windowHeight);
		camera.Input();


		scene.Draw(camera);


		display.EndFrame();
	}

	

	return 0;
}