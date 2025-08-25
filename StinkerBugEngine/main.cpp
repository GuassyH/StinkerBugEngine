#include <cstdint>
#include <iostream>
#include <memory>
#include <unordered_map>

#include "Display.h"
#include "Constants.h"
#include "SceneManager.h"
#include "FullScreenPass.h"

#include "ComponentsList.h"
#include "Entity.h"
#include "EntityBehaviour.h"

#include "CameraMovement.h"
#include "SphereMove.h"
#include "JumpMechanic.h"


inline void DebugVec3(std::string text, glm::vec3 vector) {
	std::cout << text << ": " << vector.x << "x " << vector.y << "y " << vector.z << "z\n";
}


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
	Material test_mat; test_mat.Color = Constants::Colors::Purple;
	
	Entity camera = scene.CreateEntity();
	camera.GetComponent<Transform>().position = glm::vec3(0.0, 2.0, 10);
	camera.AddComponent<Camera>(display.windowWidth, display.windowHeight, camera.GetComponent<Transform>()).main = true;
	camera.AddComponent<CameraMovement>();
	
	FullScreenPass skybox_pass = FullScreenPass(camera.GetComponent<Camera>(), skybox_mat);

	Mesh floor = Mesh(Constants::Shapes::Plane());
	Mesh sphere = Mesh(Constants::Shapes::UVSphere());
	Mesh cube = Mesh(Constants::Shapes::Cube());
	Mesh cube2 = Mesh(Constants::Shapes::Cube());
	Mesh cube3 = Mesh(Constants::Shapes::Cube());
	Mesh test_cube = Mesh(Constants::Shapes::Cube());

	// ALL ENTITIES

	Entity& big_Light = scene.CreateEntity();
	big_Light.AddComponent<Light>().light_type = LightTypes::Spotlight;
	big_Light.GetComponent<Light>().color = glm::vec4(0.4f);
	big_Light.GetComponent<Transform>().rotation = scene.light_direction;
	big_Light.GetComponent<Transform>().position = -big_Light.GetComponent<Transform>().rotation * glm::vec3(100);
	
	Entity& e_plane = scene.CreateEntity();
	e_plane.GetComponent<Transform>().scale = glm::vec3(250);
	e_plane.AddComponent<MeshRenderer>(floor, material);

	
	Entity& e_globe = scene.CreateEntity();
	e_globe.GetComponent<Transform>().position = glm::vec3(0, 5, 0);
	e_globe.AddComponent<MeshRenderer>(sphere, material);
	e_globe.AddComponent<RigidBody>();
	e_globe.AddComponent<SphereCollider>();
	e_globe.GetComponent<SphereCollider>().radius = 0.5f;
	

	Entity& e_cube = scene.CreateEntity();
	e_cube.GetComponent<Transform>().position = glm::vec3(-2, 5, 0);
	e_cube.AddComponent<RigidBody>().mass = 1.0f;
	e_cube.AddComponent<MeshRenderer>(cube, red);
	e_cube.AddComponent<BoxCollider>();
	e_cube.AddComponent<JumpMechanic>();

	Entity& e_cube_2 = scene.CreateEntity();
	e_cube_2.GetComponent<Transform>().position = glm::vec3(-4, 5, 0);
	e_cube_2.GetComponent<Transform>().rotation = glm::vec3(0, 70, 0);
	e_cube_2.AddComponent<RigidBody>().mass = 1.0f;
	e_cube_2.AddComponent<MeshRenderer>(cube2, blue);
	e_cube_2.AddComponent<BoxCollider>();
	e_cube_2.AddComponent<SphereMove>();

	Entity& e_cube_3 = scene.CreateEntity();
	e_cube_3.GetComponent<Transform>().position = glm::vec3(2, 5, 0);
	e_cube_3.GetComponent<Transform>().rotation = glm::vec3(0, 25, 0);
	e_cube_3.GetComponent<Transform>().scale = glm::vec3(2);
	e_cube_3.AddComponent<RigidBody>().mass = 10.0f;
	e_cube_3.AddComponent<MeshRenderer>(cube3, blue);
	e_cube_3.AddComponent<BoxCollider>().size = glm::vec3(2);

	Entity& e_test_cube = scene.CreateEntity();
	e_test_cube.AddComponent<MeshRenderer>(test_cube, test_mat);
	e_test_cube.GetComponent<Transform>().rotation = glm::vec3(15, 0, 0);
	e_test_cube.GetComponent<Transform>().position = glm::vec3(0, 2, -10);
	e_test_cube.GetComponent<Transform>().scale = glm::vec3(2, 3, 2);
	e_test_cube.AddComponent<RigidBody>().isKinematic = true;
	e_test_cube.AddComponent<BoxCollider>().size = glm::vec3(2, 3, 2);

	Camera& camera_component = camera.GetComponent<Camera>();

	scene.StartEntityBehaviours();
	scene.WakeEntityBehaviours();
	while (!glfwWindowShouldClose(display.window)) {
		display.BeginFrame();
		skybox_pass.Draw(camera_component);

		Transform& light_t = big_Light.GetComponent<Transform>();
		e_test_cube.GetComponent<Transform>().rotation.y += 0.1f;
		e_test_cube.GetComponent<Transform>().rotation.x += 0.1f;


		camera_component.UpdateMatrix(75.0f, 0.1f, 1000.0f, display.windowWidth, display.windowHeight);
		camera_component.Render(big_Light.GetComponent<Light>(), light_t);

		if (glfwGetKey(display.window, GLFW_KEY_K) == GLFW_PRESS) { e_cube.GetComponent<Transform>().rotation.y += 0.1f; }

		scene.UpdateEntityBehaviours();
		scene.UpdatePhysics();


		display.EndFrame();
	}

	sceneManager.UnloadScene(scene);

	return 0;
}