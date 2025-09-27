#include "SceneManager.h"
#include "Scene.h"
#include "Entity.h"
#include "EntityHelper.h"

SceneManager& SceneManager::getInstance(){
	static SceneManager instance; return instance;
}

// Set active scene to given scene, and deloads previous scene
void SceneManager::SetActiveScene(Scene& scene) {
	activeScene = &scene;
}

// Try to get the active scene, if you cant dont
Scene& SceneManager::GetActiveScene() {
	if (!activeScene) {
		throw std::runtime_error("No active scene loaded.");
	}
	return *activeScene;
}

void SceneManager::UnloadScene() {
	Scene& scene = GetActiveScene();
	for (auto& [id, components_renderer] : scene.Scene_ECS.WorldRegistry.GetComponentMap<MeshRenderer>()) {
		MeshRenderer& renderer = *std::static_pointer_cast<MeshRenderer>(components_renderer);
		if (renderer.model) {
			renderer.model->~Model();
		}
		if (renderer.material) {
			glDeleteProgram(renderer.material->shader.ID);
			renderer.material->~Material();
		}
	}
}

void SceneManager::UnloadScene(Scene& scene) {
	for (auto& [id, components_renderer] : scene.Scene_ECS.WorldRegistry.GetComponentMap<MeshRenderer>()) {
		MeshRenderer& renderer = *std::static_pointer_cast<MeshRenderer>(components_renderer);
		if (renderer.model) {
			renderer.model->cleanup();
			renderer.model->~Model();
		}
		if (renderer.material) {
			glDeleteProgram(renderer.material->shader.ID);
			renderer.material->~Material();
		}
	}
}