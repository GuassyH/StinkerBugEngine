#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "Scene.h"
#include "Camera.h"
#include "Mesh.h"

// Stores an actice Scene
class SceneManager {
private:
	Scene* activeScene = nullptr;
	SceneManager() = default;
public:
	static SceneManager& getInstance(){ static SceneManager instance; return instance; }

	// Prevent copies
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;

	
	/*
	void UnloadScene() {
	}
	*/
	
	// Set active scene to given scene, and deloads previous scene
	void LoadScene(Scene& scene) {
		activeScene = &scene;
	}

	// Try to get the active scene, if you cant dont
	Scene* GetActiveScene() {
		if (!activeScene) {
			throw std::runtime_error("No active scene loaded.");
		}
		return activeScene;
	}

	// Adds mesh to the scenes list
	void RegisterMesh(Mesh* mesh) {
		if (activeScene) {
			activeScene->AddMesh(mesh);
		}
	}


	// Draws the scene
	void DrawScene(Camera& camera);
};


#endif
