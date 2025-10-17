#include "Scene.h"
#include "DeltaTime.h"
#include "CollisionInfo.h"
#include "EntityBehaviour.h"
#include "ComponentTypeID.h"
#include "Display.h"
#include "EntityObject.h"
#include "Scene.h"

DeltaTime& deltaTime = DeltaTime::getInstance();

// Create the Entity
EntityObject& Scene::CreateEntity() {
	EntityObject new_ntt;
	Entity& entity_id = Scene_ECS.nextEntity;	Scene_ECS.nextEntity++;
	Scene_ECS.component_bits[entity_id] = 0b0;
	Scene_ECS.entity_names[entity_id] = "Entity: " + std::to_string(entity_id);
	Scene_ECS.AddComponent<Transform>(entity_id, glm::vec3(0.0), glm::vec3(0.0), glm::vec3(1.0));
	Scene_ECS.entities.insert(entity_id);
	new_ntt.transform = Scene_ECS.GetComponentPtr<Transform>(entity_id);
	return new_ntt;
}

EntityObject& Scene::CreateEntity(std::string name) {
	EntityObject new_ntt;
	Entity& entity_id = Scene_ECS.nextEntity;	Scene_ECS.nextEntity++;
	Scene_ECS.component_bits[entity_id] = 0b0;
	Scene_ECS.entity_names[entity_id] = name;
	Scene_ECS.AddComponent<Transform>(entity_id, glm::vec3(0.0), glm::vec3(0.0), glm::vec3(1.0));
	Scene_ECS.entities.insert(entity_id);
	new_ntt.transform = Scene_ECS.GetComponentPtr<Transform>(entity_id);
	return new_ntt;
}


// Resolve collision (apply forces)
void Scene::ResolveCollision(CollisionInfo collision_info, RigidBody& rb1, RigidBody& rb2) {
	if (collision_info.normal != glm::vec3(0.0)) {
		glm::vec3 relative_velocity = rb1.velocity - rb2.velocity;
		float vel_along_normal = glm::dot(relative_velocity, collision_info.normal);

		if (vel_along_normal < 0) return;

		float invMass1 = (rb1.mass > 0) ? 1.0f / rb1.mass : 0.0f;
		float invMass2 = (rb2.mass > 0) ? 1.0f / rb2.mass : 0.0f;

		float j = -(1.0f + ((rb1.bounciness + rb2.bounciness) / 2.0f)) * vel_along_normal;
		j /= (invMass1 + invMass2);

		glm::vec3 impulse = j * collision_info.normal;
		if (!rb1.isKinematic) { rb1.velocity += impulse * invMass1; }
		if (!rb2.isKinematic) { rb2.velocity -= impulse * invMass2; }

		// positional corrections
		if (invMass1 + invMass2 > 0) {
			glm::vec3 correction = collision_info.normal * collision_info.penetration / (invMass1 + invMass2);
			if (!rb1.isKinematic) { rb1.transform->position -= correction * invMass1; }
			if (!rb2.isKinematic) { rb2.transform->position += correction * invMass2; }
		}
	}
}

// Check for collisions between all colliders
// Add not double checking
void Scene::CheckCollisions(uint32_t id) {
	if (Scene_ECS.colliders.contains(id)) {
		auto& this_collider = Scene_ECS.colliders[id];
		for (auto& [id2, other_collider] : Scene_ECS.colliders) {
			// if both ptrs arent null and arent the same collider
			if (this_collider != other_collider) {
				if (Scene_ECS.GetComponentMap<RigidBody>().contains(id2)) {
					CollisionInfo collision_info = this_collider->CheckCollisions(*other_collider);
					if (collision_info.did_collide) {
						ResolveCollision(collision_info, Scene_ECS.GetComponent<RigidBody>(id), Scene_ECS.GetComponent<RigidBody>(id2));
					}
				}
			}
		}
	}
}

// Update all the RigidBodies
void Scene::UpdatePhysics() {
	for (auto& [id, components_rb] : Scene_ECS.GetComponentMap<RigidBody>()) {
		RigidBody& rb = *std::static_pointer_cast<RigidBody>(components_rb);
		if (!rb.isKinematic && rb.useGravity) { rb.velocity.y += gravity * deltaTime.get(); }

		CheckCollisions(id);

		if (rb.isKinematic) { continue; }
		rb.velocity -= (rb.velocity * rb.drag) * deltaTime.get();

		rb.transform->position += rb.velocity * deltaTime.get();
	}
}

// Check if a main light exists (directional light)
bool Scene::HasMainLight() {
	if (!main_light) { return false; }
	if (!main_light->transform) { return false; }
	return (main_light && Scene_ECS.HasComponent<Light>(main_light->transform->entity));
}

// Check if a main camera exists
bool Scene::HasMainCamera() {
	if (!main_camera) { return false; }
	if (!main_camera->transform) { return false; }
	return (main_camera && Scene_ECS.HasComponent<Camera>(main_camera->transform->entity));
}

// Check all mains and set needed data
void Scene::CheckMains() {
	if (!HasMainLight()) {
		if (!main_light) {
			main_light = new EntityObject();
		}
		for (auto& [id, lightPtr] : Scene_ECS.GetComponentMap<Light>()) {
			Light* light = dynamic_cast<Light*>(lightPtr.get());
			if (light->light_type == LightTypes::Directional) {
				if (Scene_ECS.HasComponent<Transform>(id)) {
					main_light->transform = Scene_ECS.GetComponentPtr<Transform>(id);
					break;
				}
			}
		}
	}
	if (!HasMainCamera()) {
		if (!main_camera) {
			main_camera = new EntityObject();
		}
		for (auto& [id, camPtr] : Scene_ECS.GetComponentMap<Camera>()) {
			if (Scene_ECS.HasComponent<Transform>(id)) {
				std::cout << "new main_cam id set: " << id << "\n";
				main_camera->transform = Scene_ECS.GetComponentPtr<Transform>(id);
				break;
			}
		}
	}
}

// Render each camera
void Scene::Render() {
	Display& display = Display::getInstance();
	Renderer& renderer = Renderer::getInstance();

	if (renderer.queue_mesh_rebuild == true) { renderer.rebuildMeshLists(Scene_ECS.components); }
	if (renderer.queue_lights_rebuild == true) { renderer.recalculateLightsBuffer(Scene_ECS.components); }

	CheckMains();
	
	// main_camera->GetComponent<Camera>().Render(this);
}

// Call the EntityBehaviours Start
void Scene::StartEntityBehaviours() {
	for (auto& [id, map] : Scene_ECS.entity_behaviours) 
		for (auto& [typeIdx, behaviour] : map) 
			if (behaviour) behaviour->Start();
}

// Call the EntityBehaviours Awake
void Scene::WakeEntityBehaviours() {
	for (auto& [id, map] : Scene_ECS.entity_behaviours) 
		for (auto& [typeIdx, behaviour] : map) 
			if(behaviour) behaviour->Awake();
}

// Update all the EntityBehaviour scripts
void Scene::UpdateEntityBehaviours() {
	for (auto& [id, map] : Scene_ECS.entity_behaviours) 
		for (auto& [typeIdx, behaviour] : map) 
			if (behaviour) behaviour->Update();

}