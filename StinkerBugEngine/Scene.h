#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <iostream>
#include <memory>

#include "unordered_map"
#include "glm/glm.hpp"

#include "ComponentsList.h"

#include "ECSystem.h"
#include "Entity.h"
#include "FullScreenPass.h"

class EntityHelper;

class Scene {
public:
	Scene() = default;
	Entity& CreateEntity();
	Entity& CreateEntity(std::string name);

	// Physics
	float gravity = -9.82f;

	// Environment
	float ambient = 0.2f;
	EntityHelper* main_light;
	ECSystem Scene_ECS;
	std::vector<FullScreenPass> passes;

	void Render();
	void CheckCollisions(uint32_t id);
	void ResolveCollision(CollisionInfo collision_info, RigidBody& rb1, Transform& t1, RigidBody& rb2, Transform& t2);
	void UpdatePhysics();

	bool HasMainLight();

	void StartEntityBehaviours();
	void WakeEntityBehaviours();
	void UpdateEntityBehaviours();
};



#endif  