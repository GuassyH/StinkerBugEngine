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
#include "Model.h"

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
	
	Mesh floor = Mesh(Constants::Shapes::Plane());
	Mesh sphere = Mesh(Constants::Shapes::UVSphere());
	Mesh cube = Mesh(Constants::Shapes::Cube()); cube.name = "Cube";


	EntityHelper camera(scene.CreateEntity("Camera"), &scene.Scene_ECS);
	camera.GetComponent<Transform>().position = glm::vec3(0.0, 2.0, 10);
	camera.AddComponent<Camera>(display.windowWidth, display.windowHeight, camera.GetComponent<Transform>()).main = true;
	camera.AddComponent<CameraMovement>();
	
	FullScreenPass skybox_pass = FullScreenPass(camera.GetComponent<Camera>(), skybox_mat);

	Model test_model("assets/models/test_model.obj");
	Mesh test_model_mesh = Mesh(test_model);

	// ALL ENTITIES
	EntityHelper big_Light(scene.CreateEntity("Sun Light"), &scene.Scene_ECS);
	big_Light.AddComponent<Light>().light_type = LightTypes::Directional;
	big_Light.GetComponent<Light>().color = glm::vec4(0.4f);
	big_Light.GetComponent<Transform>().rotation = scene.light_direction;
	big_Light.GetComponent<Transform>().position = -big_Light.GetComponent<Transform>().rotation * glm::vec3(100);

	EntityHelper e_plane(scene.CreateEntity("Floor"), &scene.Scene_ECS);
	e_plane.GetComponent<Transform>().scale = glm::vec3(250);
	e_plane.AddComponent<MeshRenderer>(floor, material);

	EntityHelper e_model_test(scene.CreateEntity("Test Model"), &scene.Scene_ECS);
	e_model_test.AddComponent<MeshRenderer>(test_model_mesh, blue);
	e_model_test.GetComponent<Transform>().position = glm::vec3(0, 0.5, 0);

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

		camera_component.UpdateMatrix(display.windowWidth, display.windowHeight);
		camera_component.Render(big_Light.GetComponent<Light>(), light_t);


		active_scene.UpdateEntityBehaviours();
		active_scene.UpdatePhysics();

		ui.imgui_render(camera_movement, active_scene);
		display.EndFrame();
	}

	sceneManager.UnloadScene(scene);
	skybox_pass.~FullScreenPass();
	ui.imgui_shutdown();
	display.~Display();

	e_model_test.~EntityHelper();
	e_plane.~EntityHelper();
	camera.~EntityHelper();
	big_Light.~EntityHelper();

	
	return 0;
}