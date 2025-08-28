#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

class Scene;

// Stores an actice Scene
class SceneManager {
private:
	Scene* activeScene = nullptr;
	SceneManager() = default;
public:
	static SceneManager& getInstance();

	// Prevent copies
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;

	
	void SetActiveScene(Scene& scene);	// Set active scene to given scene, and deloads previous scene
	Scene& GetActiveScene();			// Try to get the active scene, if you cant dont

	void UnloadScene();
	void UnloadScene(Scene& scene);
};


#endif
