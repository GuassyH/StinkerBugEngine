#include <cstdint>
#include <iostream>
#include <memory>
#include <unordered_map>

#include "Display.h"
#include "Constants.h"
#include "Scene.h"
#include "SceneManager.h"
#include "FullScreenPass.h"

#include "ComponentsList.h"
#include "Entity.h"
#include "EntityBehaviour.h"

#include "Mesh.h"
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
	Material material; material.Color = Constants::Colors::White;
	Material red; red.Color = Constants::Colors::Red;
	Material blue; blue.Color = Constants::Colors::Blue;

	Mesh floor = Mesh(Constants::Shapes::Plane());
	Mesh cube = Mesh(Constants::Shapes::Cube());
	Mesh sphere = Mesh(Constants::Shapes::UVSphere());

	Entity camera = scene.CreateEntity();
	camera.GetComponent<Transform>().position = glm::vec3(0.0, 2.0, 10);
	camera.AddComponent<Camera>(display.windowWidth, display.windowHeight, camera.GetComponent<Transform>()).main = true;
	camera.AddComponent<CameraMovement>();

	FullScreenPass skybox_pass = FullScreenPass(camera.GetComponent<Camera>(), skybox_mat);

	Entity big_Light = scene.CreateEntity();
	big_Light.AddComponent<Light>().light_type = LightTypes::Spotlight;
	big_Light.GetComponent<Light>().color = glm::vec4(0.4f);
	
	Entity e_plane = scene.CreateEntity();
	e_plane.GetComponent<Transform>().position = glm::vec3(-5, 0, -5);
	e_plane.GetComponent<Transform>().scale = glm::vec3(10);
	e_plane.AddComponent<MeshRenderer>(floor, material);

	Entity e_globe = scene.CreateEntity();
	e_globe.GetComponent<Transform>().position = glm::vec3(-1, 10, 0);
	e_globe.AddComponent<MeshRenderer>(sphere, material);
	e_globe.AddComponent<RigidBody>();
	e_globe.AddComponent<SphereCollider>().radius = 0.5f;
	e_globe.AddComponent<SphereMove>();

	Entity e_globe_2 = scene.CreateEntity();
	e_globe_2.GetComponent<Transform>().position = glm::vec3(1, 10, 0);
	e_globe_2.AddComponent<MeshRenderer>(sphere, red);
	e_globe_2.AddComponent<RigidBody>();
	e_globe_2.AddComponent<SphereCollider>().radius = 0.5f;
	e_globe_2.AddComponent<JumpMechanic>();


	for (int i = 0; i < 10; i++)
	{
		for (int p = 0; p < 10; p++)
		{
			Entity e_big_globe = scene.CreateEntity();
			e_big_globe.GetComponent<Transform>().position = glm::vec3(i - 5, 5, p - 5);
			e_big_globe.GetComponent<Transform>().scale = glm::vec3(1.0);
			e_big_globe.AddComponent<MeshRenderer>(sphere, blue);
			e_big_globe.AddComponent<RigidBody>().mass = 2;
			e_big_globe.AddComponent<SphereCollider>().radius = 0.5f;
		}
	}




	Camera& camera_component = camera.GetComponent<Camera>();

	scene.StartEntityBehaviours();
	scene.WakeEntityBehaviours();
	while (!glfwWindowShouldClose(display.window)) {
		display.BeginFrame();
		skybox_pass.Draw(camera_component);


		camera_component.UpdateMatrix(75.0f, 0.1f, 1000.0f, display.windowWidth, display.windowHeight);
		camera_component.Render(big_Light.GetComponent<Light>(), big_Light.GetComponent<Transform>());


		scene.UpdateEntityBehaviours();
		scene.UpdatePhysics();


		display.EndFrame();
	}

	sceneManager.UnloadScene(scene);

	return 0;
}