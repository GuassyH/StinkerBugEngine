#include <iostream>

#include "Display.h"
#include "Constants.h"
#include "SceneManager.h"
#include "Scene.h"
#include "FullScreenPass.h"

#include "Entity.h"

#include "Mesh.h"
#include "MeshRenderer.h"
#include "Collider.h"
#include "SphereCollider.h"



int main(void) {
	Display& display = Display::getInstance();
	if (display.init(1920, 1080, "Stinker Bug Engine") == -1) { return -1; }

	Camera camera = Camera(display.windowWidth, display.windowHeight, glm::vec3(0.0));
	camera.transform.position = glm::vec3(0, 3, 10.0f);

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
	Mesh sphere = Mesh(Constants::Shapes::UVSphere());

	FullScreenPass skybox_pass = FullScreenPass(camera, skybox_mat);

	Entity e_floor = scene.CreateEntity();
	scene.AddComponent<MeshRenderer>(e_floor, floor, material);
	scene.AddComponent<RigidBody>(e_floor).isKinematic = true;
	scene.GetComponent<Transform>(e_floor).position = glm::vec3(-5.0, 0.0, -5.0);
	scene.GetComponent<Transform>(e_floor).scale = glm::vec3(10.0);

	Entity e_globe = scene.CreateEntity();
	scene.AddComponent<MeshRenderer>(e_globe, sphere, material);
	scene.AddComponent<RigidBody>(e_globe).useGravity = true;
	scene.GetComponent<Transform>(e_globe).position = glm::vec3(1.0, 2, 0.0);
	scene.AddComponent<SphereCollider>(e_globe);

	Entity e_globe2 = scene.CreateEntity();
	scene.AddComponent<MeshRenderer>(e_globe2, sphere, material);
	scene.AddComponent<RigidBody>(e_globe2).useGravity = true;
	scene.GetComponent<Transform>(e_globe2).position = glm::vec3(-1.0, 2, 0.0);
	scene.AddComponent<SphereCollider>(e_globe2);

	Entity e_globe3 = scene.CreateEntity();
	scene.AddComponent<MeshRenderer>(e_globe3, sphere, material);
	scene.AddComponent<RigidBody>(e_globe3).useGravity = true;
	scene.GetComponent<Transform>(e_globe3).position = glm::vec3(3.0, 2, 0.0);
	scene.AddComponent<SphereCollider>(e_globe3);


	while (!glfwWindowShouldClose(display.window)) {
		display.BeginFrame();
		skybox_pass.Draw(camera);

		if (glfwGetKey(display.window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			scene.GetComponent<RigidBody>(e_globe2).velocity += glm::vec3(5.0, 0.0, 0.0) * deltaTime.get();
		}
		if (glfwGetKey(display.window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			scene.GetComponent<RigidBody>(e_globe2).velocity += glm::vec3(-5.0, 0.0, 0.0) * deltaTime.get();
		}

		camera.UpdateMatrix(75.0f, 0.1f, 1000.0f, display.windowWidth, display.windowHeight);
		camera.Input();

		scene.UpdatePhysics();
		scene.DrawMeshes(camera);


		display.EndFrame();
	}

	

	return 0;
}