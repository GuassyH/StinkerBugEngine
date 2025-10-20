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

class EntityObject;
class Transform;

class Scene {
private:
	bool name_exists(std::string name);
public:
	Scene() = default;
	EntityObject& CreateEntity();
	EntityObject& CreateEntity(std::string name);
	float ambient = 0.2f;

	// Physics
	float gravity = -9.82f;

	// Environment
	EntityObject* main_light = nullptr;
	EntityObject* main_camera = nullptr;
	ECSystem Scene_ECS;

	Shader skybox_shader = Shader("skybox.vert", "skybox.frag");
	Material skybox_mat = Material(skybox_shader);
	FullScreenPass skybox_pass = FullScreenPass(skybox_mat);
	std::vector<FullScreenPass> passes;
	

	void Render();
	void CheckCollisions(uint32_t id);
	void ResolveCollision(CollisionInfo collision_info, RigidBody& rb1, RigidBody& rb2);
	void UpdatePhysics();

	bool HasMainLight();
	bool HasMainCamera();
	void CheckMains();

	void StartEntityBehaviours();
	void WakeEntityBehaviours();
	void UpdateEntityBehaviours();
};



#endif  