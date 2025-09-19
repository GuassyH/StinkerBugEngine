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
	
	EntityHelper camera(scene.CreateEntity("Camera"), &scene.Scene_ECS);
	camera.GetComponent<Transform>().position = glm::vec3(0.0, 2.0, 10);
	camera.AddComponent<Camera>(display.windowWidth, display.windowHeight, camera.GetComponent<Transform>()).main = true;
	camera.AddComponent<CameraMovement>();
	Camera& camera_component = camera.GetComponent<Camera>();
	CameraMovement& camera_movement = camera.GetComponent<CameraMovement>();
	
	FullScreenPass skybox_pass = FullScreenPass(camera.GetComponent<Camera>(), skybox_mat);

	Scene& active_scene = sceneManager.GetActiveScene();
	active_scene.StartEntityBehaviours();
	active_scene.WakeEntityBehaviours();
	while (!glfwWindowShouldClose(display.window)) {
		display.BeginFrame();

		if (active_scene.HasMainLight()) {
			skybox_pass.Draw(camera_component, &scene.main_light->GetComponent<Light>(), &scene.main_light->GetComponent<Transform>());
		}
		

		active_scene.UpdateEntityBehaviours();
		active_scene.UpdatePhysics();
		active_scene.Render();

		ui.imgui_render(camera_movement, active_scene);
		display.EndFrame();
	}

	sceneManager.UnloadScene(scene);
	skybox_pass.~FullScreenPass();
	ui.imgui_shutdown();
	display.~Display();

	camera.~EntityHelper();

	
	return 0;
}