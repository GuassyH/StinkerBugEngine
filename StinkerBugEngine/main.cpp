#include <iostream>

#include "Display.h"
#include "Constants.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Mesh.h"
#include "FullScreenPass.h"

#include "Entity.h"

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

	Entity e_floor = scene.CreateEntity();
	// scene.AddComponent<MeshRenderer>(e_floor, floor, red);
	scene.AddComponent<MeshRenderer>(e_floor, floor, red);
	
	scene.transforms[e_floor.id].position = glm::vec3(-5.0, 0.0, -5.0);
	scene.transforms[e_floor.id].scale = glm::vec3(10.0);

	Entity e_cube = scene.CreateEntity();
	scene.AddComponent<MeshRenderer>(e_cube, cube, material);

	while (!glfwWindowShouldClose(display.window)) {
		display.BeginFrame();
		skybox_pass.Draw(camera);


		camera.UpdateMatrix(75.0f, 0.1f, 1000.0f, display.windowWidth, display.windowHeight);
		camera.Input();

		scene.CheckCollisions();
		scene.DrawMeshes(camera);


		display.EndFrame();
	}

	

	return 0;
}