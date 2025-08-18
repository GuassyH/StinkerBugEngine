#include <cstdint>
#include <iostream>
#include <memory>
#include <unordered_map>

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
#include "EntityBehaviour.h"
#include "SphereMove.h"


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
	e_floor.AddComponent<MeshRenderer>(floor, material);
	e_floor.GetComponent<Transform>().scale = glm::vec3(10);
	e_floor.GetComponent<Transform>().position = glm::vec3(-5, 0, -5);

	Entity e_ball_1 = scene.CreateEntity();
	e_ball_1.AddComponent<RigidBody>();
	e_ball_1.AddComponent<SphereCollider>();
	e_ball_1.GetComponent<Transform>().position = glm::vec3(0.0, 10, 0.0);
	e_ball_1.AddComponent<MeshRenderer>(sphere, red);
	Transform& t = e_ball_1.GetComponent<Transform>();
	RigidBody& rb = e_ball_1.GetComponent<RigidBody>();
	e_ball_1.AddComponent<SphereMove>(t, rb);

	scene.StartEntityBehaviours();
	scene.WakeEntityBehaviours();
	bool shot = false;
	while (!glfwWindowShouldClose(display.window)) {
		display.BeginFrame();
		skybox_pass.Draw(camera);

		
		

		camera.UpdateMatrix(75.0f, 0.1f, 1000.0f, display.windowWidth, display.windowHeight);
		camera.Input();

		scene.UpdateEntityBehaviours();
		scene.UpdatePhysics();
		scene.DrawMeshes(camera);


		display.EndFrame();
	}

	

	return 0;
}