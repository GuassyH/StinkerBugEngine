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

	SceneManager& sceneManager = SceneManager::getInstance();
	Scene scene;
	sceneManager.SetActiveScene(scene);

	UI& ui = UI::getInstance();
	ui.imgui_init();

	Shader skybox_shader("skybox.vert", "skybox.frag");
	Material skybox_mat(skybox_shader);
	
	/*
	EntityHelper camera(scene.CreateEntity("Camera"), &scene.Scene_ECS);
	camera.GetComponent<Transform>().position = glm::vec3(0.0, 2.0, 10);
	camera.AddComponent<Camera>(display.windowWidth, display.windowHeight, camera.GetComponent<Transform>()).main = true;
	camera.AddComponent<CameraMovement>();
	Camera& camera_component = camera.GetComponent<Camera>();
	CameraMovement& camera_movement = camera.GetComponent<CameraMovement>();
	
	FullScreenPass skybox_pass = FullScreenPass(camera_component, skybox_mat);
	*/


	EntityHelper dir_light(scene.CreateEntity(), &scene.Scene_ECS);
	dir_light.AddComponent<Light>().light_type = LightTypes::Directional;
	dir_light.GetComponent<Transform>().rotation = glm::vec3(-55.0f, 15.0f, 0.0f);

	Scene& active_scene = sceneManager.GetActiveScene();
	active_scene.StartEntityBehaviours();
	active_scene.WakeEntityBehaviours();
	while (!glfwWindowShouldClose(display.window)) {
		display.BeginFrame();

		if (active_scene.HasMainLight()) {
			// skybox_pass.Draw(camera_component, &scene.main_light->GetComponent<Light>(), &scene.main_light->GetComponent<Transform>());
		}
		

		active_scene.UpdateEntityBehaviours();
		active_scene.UpdatePhysics();
		active_scene.Render();

		ui.imgui_render(active_scene);
		display.EndFrame();
	}

	sceneManager.UnloadScene(scene);
	ui.imgui_shutdown();
	display.~Display();


	
	return 0;
}