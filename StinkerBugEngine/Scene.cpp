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

void Scene::ResolveCollision(glm::vec3 collision_normal, RigidBody& rb1, RigidBody&  rb2){
	if (collision_normal != glm::vec3(0.0)) {
		glm::vec3 relative_velocity = rb1.velocity - rb2.velocity;
		float vel_along_normal = glm::dot(relative_velocity, collision_normal);

		if (vel_along_normal < 0) return;

		float invMass1 = (rb1.mass > 0) ? 1.0f / rb1.mass : 0.0f;
		float invMass2 = (rb2.mass > 0) ? 1.0f / rb2.mass : 0.0f;

		float j = -(1.0f + ((rb1.bounciness + rb2.bounciness) / 2.0f)) * vel_along_normal;
		j /= (invMass1 + invMass2);

		glm::vec3 impulse = j * collision_normal;
		rb1.velocity += impulse * invMass1;
		rb2.velocity -= impulse * invMass2;
	}
}

void Scene::CheckCollisions(uint32_t id) {
	if (colliders.find(id) != colliders.end()) {
		SphereCollider& this_collider = colliders[id];
		this_collider.radius = transforms[id].scale.x / 2.0;
		this_collider.position = transforms[id].position;

		for (auto& [id2, other_collider] : colliders) {
			// if both ptrs arent null and arent the same collider
			if (&this_collider != &other_collider) {
				other_collider.radius = transforms[id2].scale.x / 2.0;
				other_collider.position = transforms[id2].position;

				glm::vec3 collision_normal = this_collider.CheckCollisions(other_collider);
				ResolveCollision(collision_normal, rigidbodies[id], rigidbodies[id2]);
			}
		}
	}
	else {
		return;
	}
}

void Scene::UpdatePhysics() {
	for (auto& [id, rb] : rigidbodies) {
		if (rb.isKinematic) { continue; }
		if (rb.useGravity) { rb.velocity.y += gravity * deltaTime.get(); }
		
		CheckCollisions(id);

		rb.velocity -= rb.velocity * (1.0f - rb.drag) * deltaTime.get();


		Transform& transform = transforms[id];
		transform.position += rb.velocity * deltaTime.get();
		
		// Temp Floor collision
		if (transform.position.y < 0.0f && transform.position.x > -6 && transform.position.x < 4 && transform.position.z > -6 && transform.position.z < 4) { transform.position.y = 0.0f; rb.velocity.y = 0.0f; }
	}
}

