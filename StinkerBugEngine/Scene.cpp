#include "Scene.h"
#include "Entity.h"
#include "DeltaTime.h"

DeltaTime& deltaTime = DeltaTime::getInstance();

Entity Scene::CreateEntity() {
	transforms[nextEntity] = { glm::vec3(0.0), glm::vec3(0.0), glm::vec3(1.0) };
	return Entity(nextEntity++, this);
}


void Scene::DrawMeshes(Camera& camera) {
	for (auto& [id, renderer] : mesh_renderers) {
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

void Scene::UpdatePhysics() {
	for (auto& [id, rb] : rigidbodies) {
		if (rb.isKinematic) { continue; }
		if (rb.useGravity) { rb.velocity.y += gravity * deltaTime.get(); }

		Transform& transform = transforms[id];
		transform.position += rb.velocity * deltaTime.get();
		if (transform.position.y < 0) { transform.position.y = 0; rb.velocity = glm::vec3(0.0); }
	}
}

