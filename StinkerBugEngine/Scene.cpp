#include "Scene.h"
#include "Entity.h"


Entity Scene::CreateEntity() {
	transforms[nextEntity] = { glm::vec3(0.0), glm::vec3(0.0), glm::vec3(1.0) };
	return Entity(nextEntity++, this);
}




void Scene::DrawMeshes(Camera& camera) {
	for (auto& [id, renderer] : meshRenderers) {
		auto it = transforms.find(id);
		Transform& t = it->second;

		renderer.mesh->Draw(camera, renderer.material, t);
	}
}

void Scene::CheckCollisions() {
	for (auto& [id, this_collider] : colliders) {
		for (auto& [id, other_collider] : colliders) {
			// if both ptrs arent null and arent the same collider
			if (this_collider && other_collider && this_collider != other_collider) {
				this_collider->CheckCollisions(*other_collider);
			}
		}
	}
}


