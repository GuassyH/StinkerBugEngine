#include <unordered_map>
#include <iostream>
#include <cstdint>
#include <memory>

#include "FullScreenPass.h"
#include "SceneManager.h"
#include "Constants.h"
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

#include "CameraMovement.h"
#include "JumpMechanic.h"
#include "SphereMove.h"


int main(void) {
	Display& display = Display::getInstance();
	if (display.init(1920, 1080, "Stinker Bug Engine") == -1) { std::cout << "Display failed init" << std::endl; return -1; }

	DeltaTime& deltaTime = DeltaTime::getInstance();

	SceneManager& sceneManager = SceneManager::getInstance();
	Scene scene;
	Scene scene_alt;
	sceneManager.SetActiveScene(scene);

	UI& ui = UI::getInstance();
	ui.imgui_init();

	EntityHelper dir_light(scene.CreateEntity("Sun Light"), &scene.Scene_ECS);
	dir_light.AddComponent<Light>().light_type = LightTypes::Directional;
	dir_light.GetComponent<Transform>().rotation = glm::vec3(50.0f, 205.0f, 0.0f);


	EntityHelper main_camera(scene.CreateEntity("Main Camera"), &scene.Scene_ECS);
	main_camera.AddComponent<Camera>(1920, 1080, main_camera.GetComponent<Transform>());
	main_camera.GetComponent<Transform>().rotation = glm::vec3(0.0f, 180.0f, 0.0f);
	main_camera.GetComponent<Transform>().position = glm::vec3(0.0f, 0.0f, 10.0f);

	EntityHelper goblin(scene.CreateEntity("Goblin Guy"), &scene.Scene_ECS);
	goblin.AddComponent<MeshRenderer>(new Model(glm::vec3(0.05f)), new Material(MaterialFlags_Lit | MaterialFlags_Depth | MaterialFlags_Shadow));
	goblin.GetComponent<MeshRenderer>().model->loadModel("assets/models/lotr_troll/scene.gltf");
	goblin.GetComponent<Transform>().scale = glm::vec3(0.05f);

	EntityHelper Floor(scene.CreateEntity("Floor"), &scene.Scene_ECS);
	Floor.AddComponent<MeshRenderer>(new Model(Constants::Shapes::Plane()), new Material(MaterialFlags_Lit | MaterialFlags_Depth | MaterialFlags_Shadow));
	Floor.GetComponent<Transform>().scale = glm::vec3(250.0f);
	Floor.GetComponent<Transform>().position = glm::vec3(0.0f, -10.0f, 0.0f);

	Scene* active_scene = &sceneManager.GetActiveScene();
	active_scene->StartEntityBehaviours();
	active_scene->WakeEntityBehaviours();
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