#include <cstdint>
#include <iostream>
#include <memory>
#include <unordered_map>

#include "Display.h"
#include "Constants.h"
#include "SceneManager.h"
#include "FullScreenPass.h"

#include "Scene.h"

#include "ComponentsList.h"
#include "Entity.h"
#include "EntityHelper.h"
#include "EntityBehaviour.h"
#include "ECSystem.h"

#include "CameraMovement.h"
#include "SphereMove.h"
#include "JumpMechanic.h"
#include "UI.h"


inline void DebugVec3(std::string text, glm::vec3 vector) {
	std::cout << text << ": " << vector.x << "x " << vector.y << "y " << vector.z << "z\n";
}


int main(void) {
	Display& display = Display::getInstance();
	if (display.init(1920, 1080, "Stinker Bug Engine") == -1) { return -1; }

	DeltaTime& deltaTime = DeltaTime::getInstance();
	UI& ui = UI::getInstance();
	ui.imgui_init();

	SceneManager& sceneManager = SceneManager::getInstance();
	Scene scene;
	sceneManager.SetActiveScene(scene);

	Shader skybox_shader("skybox.vert", "skybox.frag");
	Material skybox_mat(skybox_shader);
	Material material; material.Color = Constants::Colors::White;
	Material red; red.Color = Constants::Colors::Red;
	Material blue; blue.Color = Constants::Colors::Blue;
	Material white; white.Color = Constants::Colors::White;
	Material test_mat; test_mat.Color = Constants::Colors::Purple;
	
	Mesh floor = Mesh(Constants::Shapes::Plane());
	Mesh sphere = Mesh(Constants::Shapes::UVSphere());
	Mesh cube = Mesh(Constants::Shapes::Cube()); cube.name = "Cube";

	EntityHelper camera = EntityHelper(scene.CreateEntity("Camera"), &scene.Scene_ECS);
	camera.GetComponent<Transform>().position = glm::vec3(0.0, 2.0, 10);
	camera.AddComponent<Camera>(display.windowWidth, display.windowHeight, camera.GetComponent<Transform>()).main = true;
	camera.AddComponent<CameraMovement>();
	
	FullScreenPass skybox_pass = FullScreenPass(camera.GetComponent<Camera>(), skybox_mat);

	// ALL ENTITIES
	EntityHelper big_Light = EntityHelper(scene.CreateEntity("Sun Light"), &scene.Scene_ECS);
	big_Light.AddComponent<Light>().light_type = LightTypes::Directional;
	big_Light.GetComponent<Light>().color = glm::vec4(0.4f);
	big_Light.GetComponent<Transform>().rotation = scene.light_direction;
	big_Light.GetComponent<Transform>().position = -big_Light.GetComponent<Transform>().rotation * glm::vec3(100);

	EntityHelper e_plane = EntityHelper(scene.CreateEntity("Floor"), &scene.Scene_ECS);
	e_plane.GetComponent<Transform>().scale = glm::vec3(250);
	e_plane.AddComponent<MeshRenderer>(floor, material);
	
	EntityHelper e_cube = EntityHelper(scene.CreateEntity("Jumpable Cube"), &scene.Scene_ECS);
	e_cube.GetComponent<Transform>().position = glm::vec3(-2, 5, 0);
	e_cube.AddComponent<RigidBody>().mass = 1.0f;
	e_cube.AddComponent<MeshRenderer>(cube, red);
	e_cube.AddComponent<BoxCollider>();
	e_cube.AddComponent<JumpMechanic>();

	EntityHelper e_cube_2 = EntityHelper(scene.CreateEntity("Movable Cube"), &scene.Scene_ECS);
	e_cube_2.GetComponent<Transform>().position = glm::vec3(-4, 5, 0);
	e_cube_2.GetComponent<Transform>().rotation = glm::vec3(0, 45, 0);
	e_cube_2.AddComponent<RigidBody>().mass = 1.0f;
	e_cube_2.AddComponent<MeshRenderer>(cube, blue);
	e_cube_2.AddComponent<BoxCollider>();
	e_cube_2.AddComponent<SphereMove>();

	EntityHelper e_cube_3 = EntityHelper(scene.CreateEntity("Cube 3"), &scene.Scene_ECS);
	e_cube_3.GetComponent<Transform>().position = glm::vec3(2, 5, -.5);
	e_cube_3.GetComponent<Transform>().rotation = glm::vec3(0, 25, 0);
	e_cube_3.GetComponent<Transform>().scale = glm::vec3(2);
	e_cube_3.AddComponent<RigidBody>().mass = 2.0f;
	e_cube_3.AddComponent<MeshRenderer>(cube, blue);
	e_cube_3.AddComponent<BoxCollider>().size = glm::vec3(2);

	EntityHelper e_test_cube = EntityHelper(scene.CreateEntity("Test Cube"), &scene.Scene_ECS);
	e_test_cube.AddComponent<MeshRenderer>(cube, test_mat);
	e_test_cube.GetComponent<Transform>().rotation = glm::vec3(15, 0, 0);
	e_test_cube.GetComponent<Transform>().position = glm::vec3(0, 2, -7);
	e_test_cube.GetComponent<Transform>().scale = glm::vec3(2, 3, 2);
	e_test_cube.AddComponent<RigidBody>().isKinematic = true;
	e_test_cube.AddComponent<BoxCollider>().size = glm::vec3(2, 3, 2);
	
	Camera& camera_component = camera.GetComponent<Camera>();
	CameraMovement& camera_movement = camera.GetComponent<CameraMovement>();

	Scene& active_scene = sceneManager.GetActiveScene();
	active_scene.StartEntityBehaviours();
	active_scene.WakeEntityBehaviours();
	while (!glfwWindowShouldClose(display.window)) {
		display.BeginFrame();
		skybox_pass.Draw(camera_component);

		big_Light.GetComponent<Transform>().position = -big_Light.GetComponent<Transform>().rotation * glm::vec3(100);

		Transform& light_t = big_Light.GetComponent<Transform>();
		e_test_cube.GetComponent<Transform>().rotation.y += 0.1f;
		e_test_cube.GetComponent<Transform>().rotation.x += 0.1f;

		camera_component.UpdateMatrix(display.windowWidth, display.windowHeight);
		camera_component.Render(big_Light.GetComponent<Light>(), light_t);


		active_scene.UpdateEntityBehaviours();
		active_scene.UpdatePhysics();

		ui.imgui_render(camera_movement, active_scene, cube, material);
		display.EndFrame();
	}

	sceneManager.UnloadScene(scene);
	skybox_pass.~FullScreenPass();
	ui.imgui_shutdown();
	display.~Display();

	return 0;
}