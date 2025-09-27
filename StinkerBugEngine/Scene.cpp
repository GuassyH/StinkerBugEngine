#include "Scene.h"
#include "DeltaTime.h"
#include "CollisionInfo.h"
#include "EntityBehaviour.h"
#include "ComponentTypeID.h"
#include "Display.h"
#include "EntityHelper.h"

DeltaTime& deltaTime = DeltaTime::getInstance();

// Create the Entity
Entity& Scene::CreateEntity() {
	Entity& entity_id = Scene_ECS.WorldRegistry.nextEntity;	Scene_ECS.WorldRegistry.nextEntity++;
	Scene_ECS.WorldRegistry.component_bits[entity_id] = 0b0;
	Scene_ECS.WorldRegistry.entity_names[entity_id] = "Entity: " + std::to_string(entity_id);
	Scene_ECS.WorldRegistry.AddComponent<Transform>(entity_id, glm::vec3(0.0), glm::vec3(0.0), glm::vec3(1.0));
	Scene_ECS.WorldRegistry.entities.insert(entity_id);
	return entity_id;
}


Entity& Scene::CreateEntity(std::string name) {
	Entity& entity_id = Scene_ECS.WorldRegistry.nextEntity;	Scene_ECS.WorldRegistry.nextEntity++;
	Scene_ECS.WorldRegistry.component_bits[entity_id] = 0b0;
	Scene_ECS.WorldRegistry.entity_names[entity_id] = name;
	Scene_ECS.WorldRegistry.AddComponent<Transform>(entity_id, glm::vec3(0.0), glm::vec3(0.0), glm::vec3(1.0));
	Scene_ECS.WorldRegistry.entities.insert(entity_id);
	return entity_id;
}


// Resolve collision (apply forces)
void Scene::ResolveCollision(CollisionInfo collision_info, RigidBody& rb1, Transform& t1, RigidBody& rb2, Transform& t2) {
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
			if (!rb1.isKinematic) { t1.position -= correction * invMass1; }
			if (!rb2.isKinematic) { t2.position += correction * invMass2; }
		}
	}
}

// Check for collisions between all colliders
// Add not double checking
void Scene::CheckCollisions(uint32_t id) {
	if (Scene_ECS.WorldRegistry.colliders.find(id) != Scene_ECS.WorldRegistry.colliders.end()) {
		auto& this_collider = Scene_ECS.WorldRegistry.colliders[id];
		for (auto& [id2, other_collider] : Scene_ECS.WorldRegistry.colliders) {
			// if both ptrs arent null and arent the same collider
			if (this_collider != other_collider) {
				if (Scene_ECS.WorldRegistry.GetComponentMap<RigidBody>().find(id2) != Scene_ECS.WorldRegistry.GetComponentMap<RigidBody>().end()) {
					CollisionInfo collision_info = this_collider->CheckCollisions(*other_collider);
					if (collision_info.did_collide) {
						ResolveCollision(collision_info, Scene_ECS.WorldRegistry.GetComponent<RigidBody>(id), Scene_ECS.WorldRegistry.GetComponent<Transform>(id), Scene_ECS.WorldRegistry.GetComponent<RigidBody>(id2), Scene_ECS.WorldRegistry.GetComponent<Transform>(id2));
					}
				}
			}
		}
	}
}

// Update all the RigidBodies
void Scene::UpdatePhysics() {
	for (auto& [id, components_rb] : Scene_ECS.WorldRegistry.GetComponentMap<RigidBody>()) {
		RigidBody& rb = *std::static_pointer_cast<RigidBody>(components_rb);
		if (!rb.isKinematic && rb.useGravity) { rb.velocity.y += gravity * deltaTime.get(); }

		CheckCollisions(id);

		if (rb.isKinematic) { continue; }
		rb.velocity -= (rb.velocity * rb.drag) * deltaTime.get();

		Transform& transform = Scene_ECS.WorldRegistry.GetComponent<Transform>(id);
		transform.position += rb.velocity * deltaTime.get();
	}
}

// Check if a main light exists (directional light)
bool Scene::HasMainLight() {
	return (main_light && Scene_ECS.WorldRegistry.components[typeid(Light)].find(main_light->id) != Scene_ECS.WorldRegistry.components[typeid(Light)].end());
}

// Render each camera
void Scene::Render() {
	Display& display = Display::getInstance();
	if (!main_light) { main_light = new EntityHelper(); }
	main_light->registry = &Scene_ECS.WorldRegistry;
	main_light->id = 0;

	for (auto& [id, lightPtr] : Scene_ECS.WorldRegistry.components[typeid(Light)])
	{
		Light* light = dynamic_cast<Light*>(lightPtr.get());
		if (light->light_type == LightTypes::Directional) {
			main_light->id = id;
			continue;
		}
	}

	for (auto& [id, camPtr] : Scene_ECS.WorldRegistry.components[typeid(Camera)]) {
		Camera* c = dynamic_cast<Camera*>(camPtr.get());
		c->UpdateMatrix(display.windowWidth, display.windowHeight);
		c->Render(this);
	}
}

// Call the EntityBehaviours Start
void Scene::StartEntityBehaviours() {
	for (auto& [id, behaviour] : Scene_ECS.WorldRegistry.entity_behaviours) {
		behaviour->Start();
	}
}

// Call the EntityBehaviours Awake
void Scene::WakeEntityBehaviours() {
	for (auto& [id, behaviour] : Scene_ECS.WorldRegistry.entity_behaviours) {
		behaviour->Awake();
	}
}

// Update all the EntityBehaviour scripts
void Scene::UpdateEntityBehaviours() {
	for (auto& [id, behaviour] : Scene_ECS.WorldRegistry.entity_behaviours) {
		behaviour->Update();
	}
}