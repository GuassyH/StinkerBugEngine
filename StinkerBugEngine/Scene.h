#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <iostream>
#include <memory>

#include "unordered_map"
#include "glm/glm.hpp"

#include "ComponentsList.h"

#include "EntityBehaviour.h"
#include "ECSystem.h"

class Entity;

class Scene {
public:


	Scene() = default;
	Entity& CreateEntity();
	Entity& CreateEntity(std::string name);


	// Physics
	float gravity = -9.82f;


	// Environment
	float ambient = 0.2f;
	glm::vec3 light_direction = glm::normalize(glm::vec3(-1, -1.3, -0.84));
	glm::vec3 light_color = glm::vec3(1.0);

	ECSystem Scene_ECS;

	void DrawMeshes(Camera& camera);
	void CheckCollisions(uint32_t id);
	void ResolveCollision(glm::vec3 collision_normal, RigidBody& rb1, RigidBody& rb2);
	void UpdatePhysics();

	void StartEntityBehaviours();
	void WakeEntityBehaviours();
	void UpdateEntityBehaviours();
};



#endif  