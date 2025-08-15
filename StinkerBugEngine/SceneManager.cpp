#include "SceneManager.h"


void SceneManager::DrawScene(Camera& camera) {
	for (auto* Mesh : activeScene->meshes) {
		Mesh->Draw(camera);
	}
}