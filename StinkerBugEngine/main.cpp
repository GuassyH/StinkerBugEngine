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
#include "EntityBehaviour.h"
#include "ComponentsList.h"
#include "CameraMovement.h"

#include "SphereMove.h"
#include "JumpMechanic.h"

int main(void) {
	Display& display = Display::getInstance();
	if (display.init(1920, 1080, "Stinker Bug Engine") == -1) { return -1; }

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

	Entity camera = scene.CreateEntity();
	camera.GetComponent<Transform>().position = glm::vec3(0.0, 2.0, 10);
	camera.AddComponent<Camera>(display.windowWidth, display.windowHeight, camera.GetComponent<Transform>());
	camera.AddComponent<CameraMovement>();

	FullScreenPass skybox_pass = FullScreenPass(camera.GetComponent<Camera>(), skybox_mat);
	
	Entity e_plane = scene.CreateEntity();
	e_plane.GetComponent<Transform>().position = glm::vec3(-5, 0, -5);
	e_plane.GetComponent<Transform>().scale = glm::vec3(10);
	e_plane.AddComponent<MeshRenderer>(floor, material);

	Entity e_globe = scene.CreateEntity();
	e_globe.GetComponent<Transform>().position = glm::vec3(0, 10, 0);
	e_globe.AddComponent<MeshRenderer>(sphere, material);
	e_globe.AddComponent<RigidBody>();
	e_globe.AddComponent<SphereCollider>().radius = 0.5f;
	e_globe.AddComponent<SphereMove>();

	Entity e_globe_2 = scene.CreateEntity();
	e_globe_2.GetComponent<Transform>().position = glm::vec3(2, 10, 0);
	e_globe_2.AddComponent<MeshRenderer>(sphere, red);
	e_globe_2.AddComponent<RigidBody>();
	e_globe_2.AddComponent<SphereCollider>().radius = 0.5f;
	e_globe_2.AddComponent<JumpMechanic>();

	Camera& camera_component = camera.GetComponent<Camera>();

	scene.StartEntityBehaviours();
	scene.WakeEntityBehaviours();
	while (!glfwWindowShouldClose(display.window)) {
		display.BeginFrame();
		skybox_pass.Draw(camera_component);



		camera_component.UpdateMatrix(75.0f, 0.1f, 1000.0f, display.windowWidth, display.windowHeight);


		scene.UpdateEntityBehaviours();
		scene.UpdatePhysics();
		scene.DrawMeshes(camera_component);



		display.EndFrame();
	}

	

	return 0;
}