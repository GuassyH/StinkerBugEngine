#include "Scene.h"
#include "Entity.h"
#include "DeltaTime.h"


DeltaTime& deltaTime = DeltaTime::getInstance();

Entity Scene::CreateEntity() {
	Scene_ECS.transforms[nextEntity] = { glm::vec3(0.0), glm::vec3(0.0), glm::vec3(1.0) };
	Scene_ECS.entity_names[nextEntity] = std::to_string(nextEntity);
	return Entity(nextEntity++, this);
}


Entity Scene::CreateEntity(std::string name) {
	Scene_ECS.transforms[nextEntity] = { glm::vec3(0.0), glm::vec3(0.0), glm::vec3(1.0) };
	Scene_ECS.entity_names[nextEntity] = name;
	return Entity(nextEntity++, this);
}


void Scene::DrawMeshes(Camera& camera) {
	for (auto& [id, renderer] : Scene_ECS.mesh_renderers) {
		auto it = Scene_ECS.transforms.find(id);
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
	if (Scene_ECS.colliders.find(id) != Scene_ECS.colliders.end()) {
		auto& this_collider = Scene_ECS.colliders[id];
		for (auto& [id2, other_collider] : Scene_ECS.colliders) {
			// if both ptrs arent null and arent the same collider
			if (&this_collider != &other_collider) {

				SphereCollider* sphereCol = dynamic_cast<SphereCollider*>(&this_collider);

				if (sphereCol) {
					sphereCol->position = Scene_ECS.transforms[id].position;
				}

				glm::vec3 collision_normal = this_collider.CheckCollisions(other_collider);
				ResolveCollision(collision_normal, Scene_ECS.rigidbodies[id], Scene_ECS.rigidbodies[id2]);
			}
		}
	}
	else {
		return;
	}
}

void Scene::UpdatePhysics() {
	for (auto& [id, rb] : Scene_ECS.rigidbodies) {
		if (rb.isKinematic) { continue; }
		if (rb.useGravity) { rb.velocity.y += gravity * deltaTime.get(); }
		
		CheckCollisions(id);

		rb.velocity -= (rb.velocity * rb.drag) * deltaTime.get();


		Transform& transform = Scene_ECS.transforms[id];
		transform.position += rb.velocity * deltaTime.get();
		
		// Temp Floor collision
		if (transform.position.y < 0.0f && transform.position.x > -5.5 && transform.position.x < 4.5 && transform.position.z > -5.5 && transform.position.z < 4.5) { transform.position.y = 0.0f; rb.velocity.y = 0.0f; }
	}
}


void Scene::StartEntityBehaviours() {
	for (auto& [id, behaviour] : Scene_ECS.entity_behaviours){
		behaviour->Start();
	}
}


void Scene::WakeEntityBehaviours() {
	for (auto& [id, behaviour] : Scene_ECS.entity_behaviours) {
		behaviour->Awake();
	}
}


void Scene::UpdateEntityBehaviours() {
	for (auto& [id, behaviour] : Scene_ECS.entity_behaviours)
	{ behaviour->Update(); }	
}