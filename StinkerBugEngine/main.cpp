#include <unordered_map>
#include <iostream>
#include <cstdint>
#include <memory>

#include "FullScreenPass.h"
#include "SceneManager.h"
#include "Constants.h"
#include "Renderer.h"
#include "Display.h"
#include "Screen.h"
#include "Scene.h"
#include "UI.h"

#include "EntityBehaviour.h"
#include "ComponentsList.h"
#include "EntityHelper.h"
#include "ECSystem.h"
#include "Entity.h"
#include "Model.h"

#include "TestScript.h"

int main(void) {
	Display& display = Display::getInstance();
	if (display.Init(1920, 1080, "Stinker Bug Engine") == -1) { std::cout << "Display failed init" << std::endl; return -1; }

	DeltaTime& deltaTime = DeltaTime::getInstance();
	Renderer& renderer = Renderer::getInstance();

	SceneManager& sceneManager = SceneManager::getInstance();
	Scene scene;
	sceneManager.SetActiveScene(scene);

	UI& ui = UI::getInstance();
	ui.imgui_init();

	EntityHelper dir_light(scene.CreateEntity("Sun Light"), &scene.Scene_ECS);
	dir_light.AddComponent<Light>().light_type = LightTypes::Directional;
	dir_light.GetComponent<Transform>().rotation = glm::vec3(50.0f, 205.0f, 0.0f);

	EntityHelper main_camera(scene.CreateEntity("Main Camera"), &scene.Scene_ECS);
	main_camera.AddComponent<Camera>(1920, 1080);
	main_camera.GetComponent<Transform>().rotation = glm::vec3(0.0f, 180.0f, 0.0f);
	main_camera.GetComponent<Transform>().position = glm::vec3(0.0f, 0.0f, 10.0f);

	EntityHelper goblin(scene.CreateEntity("Goblin Guy"), &scene.Scene_ECS);
	goblin.AddComponent<MeshRenderer>(std::make_unique<Model>(glm::vec3(0.05f)), std::make_unique<Material>(MaterialFlags_Lit | MaterialFlags_Depth | MaterialFlags_Shadow));
	goblin.GetComponent<MeshRenderer>().model->loadModel("assets/models/lotr_troll/scene.gltf");
	goblin.GetComponent<Transform>().scale = glm::vec3(0.05f);

	EntityHelper Floor(scene.CreateEntity("Floor"), &scene.Scene_ECS);
	Floor.AddComponent<MeshRenderer>(std::make_unique<Model>(Constants::Shapes::Plane()), std::make_unique<Material>(MaterialFlags_Lit | MaterialFlags_Depth | MaterialFlags_Shadow));
	Floor.GetComponent<Transform>().scale = glm::vec3(250.0f);
	Floor.GetComponent<Transform>().position = glm::vec3(0.0f, -10.0f, 0.0f);
	
	EntityHelper test(scene.CreateEntity("Test"), &scene.Scene_ECS);
	test.AddComponent<MeshRenderer>(std::make_unique<Model>(Constants::Shapes::Cube()), std::make_unique<Material>(MaterialFlags_Lit | MaterialFlags_Depth | MaterialFlags_Shadow));
	test.AddComponent<TestScript>();

	Scene* active_scene = &sceneManager.GetActiveScene();
	while (!glfwWindowShouldClose(display.window)) {
		display.BeginFrame();

		active_scene->Render();
		ui.imgui_render(*active_scene);

		display.EndFrame();
	}

	sceneManager.UnloadScene();
	ui.imgui_shutdown();
	display.~Display();

	

	return 0;
}