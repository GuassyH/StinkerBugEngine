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
	Entity entity_id = Scene_ECS.nextEntity;	Scene_ECS.nextEntity++;
	Scene_ECS.component_bits[entity_id] = 0b0;
	Scene_ECS.entity_names[entity_id] = "Entity: " + std::to_string(entity_id);
	Scene_ECS.AddComponent<Transform>(entity_id, glm::vec3(0.0), glm::vec3(0.0), glm::vec3(1.0));
	Scene_ECS.entities.insert(entity_id);
	return entity_id;
}


Entity& Scene::CreateEntity(std::string name) {
	Entity entity_id = Scene_ECS.nextEntity;	Scene_ECS.nextEntity++;
	Scene_ECS.component_bits[entity_id] = 0b0;
	Scene_ECS.entity_names[entity_id] = name;
	Scene_ECS.AddComponent<Transform>(entity_id, glm::vec3(0.0), glm::vec3(0.0), glm::vec3(1.0));
	Scene_ECS.entities.insert(entity_id);
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
	if (Scene_ECS.colliders.find(id) != Scene_ECS.colliders.end()) {
		auto& this_collider = Scene_ECS.colliders[id];
		for (auto& [id2, other_collider] : Scene_ECS.colliders) {
			// if both ptrs arent null and arent the same collider
			if (this_collider != other_collider) {
				if (Scene_ECS.GetComponentMap<RigidBody>().find(id2) != Scene_ECS.GetComponentMap<RigidBody>().end()) {
					CollisionInfo collision_info = this_collider->CheckCollisions(*other_collider);
					if (collision_info.did_collide) {
						ResolveCollision(collision_info, Scene_ECS.GetComponent<RigidBody>(id), Scene_ECS.GetComponent<Transform>(id), Scene_ECS.GetComponent<RigidBody>(id2), Scene_ECS.GetComponent<Transform>(id2));
					}
				}
			}
		}

		// Temporary Plane collision
		Transform& transform = Scene_ECS.GetComponent<Transform>(id);
		if ((transform.position.y - (this_collider->size.y / 2.0f)) <= 0.0f){
			transform.position.y = (this_collider->size.y / 2.0f) + 0.001f; 
			Scene_ECS.GetComponent<RigidBody>(id).velocity.y = 0.0f;
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

		Transform& transform = Scene_ECS.GetComponent<Transform>(id);
		transform.position += rb.velocity * deltaTime.get();
	}
}


bool Scene::HasMainLight() {
	return (main_light && Scene_ECS.components[typeid(Light)].find(main_light->id) != Scene_ECS.components[typeid(Light)].end());
}

void Scene::Render() {
	Display& display = Display::getInstance();
	if (!main_light) { main_light = new EntityHelper(); }
	main_light->ecs = &Scene_ECS;
	main_light->id = 0;

	for (auto& [id, lightPtr] : Scene_ECS.components[typeid(Light)])
	{
		Light* light = dynamic_cast<Light*>(lightPtr.get());
		if (light->light_type == LightTypes::Directional) {
			main_light->id = id;
			continue;
		}
	}

	if (HasMainLight()) {
		float pitch = glm::radians(main_light->GetComponent<Transform>().rotation.x);
		float yaw = glm::radians(main_light->GetComponent<Transform>().rotation.y);

		glm::vec3 direction;
		direction.x = cos(pitch) * cos(yaw);
		direction.y = sin(pitch);
		direction.z = cos(pitch) * sin(yaw);

		main_light->GetComponent<Transform>().position = -direction * glm::vec3(100);

		
		for (auto& [id, camPtr] : Scene_ECS.components[typeid(Camera)]) {
			Camera* c = dynamic_cast<Camera*>(camPtr.get());
			c->UpdateMatrix(display.windowWidth, display.windowHeight);
			c->Render(this);
		}
	}
}

// Call the EntityBehaviours Start
void Scene::StartEntityBehaviours() {
	for (auto& [id, behaviour] : Scene_ECS.entity_behaviours) {
		behaviour->Start();
	}
}

// Call the EntityBehaviours Awake
void Scene::WakeEntityBehaviours() {
	for (auto& [id, behaviour] : Scene_ECS.entity_behaviours) {
		behaviour->Awake();
	}
}

// Update all the EntityBehaviour scripts
void Scene::UpdateEntityBehaviours() {
	for (auto& [id, behaviour] : Scene_ECS.entity_behaviours) {
		behaviour->Update();
	}
}