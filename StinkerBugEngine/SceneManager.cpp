#include "SceneManager.h"
#include "Scene.h"

SceneManager& SceneManager::getInstance(){
	static SceneManager instance; return instance;
}

// Set active scene to given scene, and deloads previous scene
void SceneManager::SetActiveScene(Scene& scene) {
	activeScene = &scene;
}

// Try to get the active scene, if you cant dont
Scene* SceneManager::GetActiveScene() {
	if (!activeScene) {
		throw std::runtime_error("No active scene loaded.");
	}
	return activeScene;
}