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
#include "EntityObject.h"
#include "ECSystem.h"
#include "Entity.h"
#include "Model.h"

#include "TestScript.h"
#include "SecondTest.h"

int main(void) {

	try {

	Display& display = Display::getInstance();
	if (display.Init(2344, 1280, "Stinker Bug Engine") == -1) { std::runtime_error("Display failed to Initialize"); }

	DeltaTime& deltaTime = DeltaTime::getInstance();
	Renderer& renderer = Renderer::getInstance();

	SceneManager& sceneManager = SceneManager::getInstance();
	Scene scene;
	sceneManager.SetActiveScene(scene);

	UI& ui = UI::getInstance();
	ui.imgui_init();
	EntityObject dir_light = scene.CreateEntity("Sun Light");
	dir_light.transform->AddComponent<Light>().light_type = LightTypes::Directional;
	dir_light.transform->rotation = glm::vec3(50.0f, 205.0f, 0.0f);

	EntityObject main_camera = scene.CreateEntity("Main Camera");
	main_camera.transform->AddComponent<Camera>(1920, 1080);
	main_camera.transform->rotation = glm::vec3(0.0f, 180.0f, 0.0f);
	main_camera.transform->position = glm::vec3(0.0f, 1.0f, 5.0f);

	EntityObject ghost = scene.CreateEntity("Stewart (Cute Ghost Guy)");
	ghost.transform->AddComponent<MeshRenderer>(new Model(), new Material(MaterialFlags_Lit | MaterialFlags_Depth | MaterialFlags_Shadow));
	ghost.transform->GetComponent<MeshRenderer>().model->loadModel("assets/models/cute_ghost/scene.gltf");
	ghost.transform->rotation = glm::vec3(-90.0f, 0.0f, 0.0f);
	ghost.transform->position.y = 0.01f;
	ghost.transform->scale = glm::vec3(2);

	EntityObject Floor = scene.CreateEntity("Floor");
	Floor.transform->AddComponent<MeshRenderer>(new Model(Constants::Shapes::Plane()), new Material(MaterialFlags_Lit | MaterialFlags_Depth | MaterialFlags_Shadow));
	Floor.transform->scale = glm::vec3(250.0f);
	// Floor.transform->AddComponent<TestScript>();
	// Floor.transform->AddComponent<SecondTest>();


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

	} catch (const std::exception& e) {
		std::cerr << "Caught exception: " << e.what() << std::endl;
		return -1;
	}

	return 0;
}