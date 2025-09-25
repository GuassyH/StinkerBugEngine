#include <unordered_map>
#include <iostream>
#include <cstdint>
#include <memory>

#include "FullScreenPass.h"
#include "SceneManager.h"
#include "Constants.h"
#include "Display.h"
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
	sceneManager.SetActiveScene(scene);

	UI& ui = UI::getInstance();
	ui.imgui_init();

	Shader skybox_shader("skybox.vert", "skybox.frag");
	Material skybox_mat(skybox_shader);

	FullScreenPass skybox_pass(skybox_mat);
	scene.passes.push_back(skybox_pass);

	EntityHelper dir_light(scene.CreateEntity("Sun Light"), &scene.Scene_ECS);
	dir_light.AddComponent<Light>().light_type = LightTypes::Directional;
	dir_light.GetComponent<Transform>().rotation = glm::vec3(-55.0f, 15.0f, 0.0f);


	EntityHelper goblin(scene.CreateEntity("Goblin"), &scene.Scene_ECS);
	goblin.AddComponent<MeshRenderer>().model = new Model(glm::vec3(0.05f));
	goblin.GetComponent<MeshRenderer>().model->loadModel("assets/models/lotr_troll/scene.gltf");
	goblin.GetComponent<MeshRenderer>().material = new Material(MaterialFlags_Lit | MaterialFlags_Depth | MaterialFlags_Shadow);
	goblin.GetComponent<Transform>().scale = goblin.GetComponent<MeshRenderer>().model->model_scale;

	Scene& active_scene = sceneManager.GetActiveScene();
	active_scene.StartEntityBehaviours();
	active_scene.WakeEntityBehaviours();
	while (!glfwWindowShouldClose(display.window)) {
		display.BeginFrame();
		
		active_scene.Render();

		ui.imgui_render(active_scene);
		display.EndFrame();
	}

	sceneManager.UnloadScene();
	ui.imgui_shutdown();
	display.~Display();

	

	return 0;
}