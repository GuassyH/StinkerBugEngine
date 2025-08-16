#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <iostream>
#include <memory>

#include "unordered_map"
#include "glm/glm.hpp"

#include "Mesh.h"
#include "Transform.h"
#include "MeshRenderer.h"
#include "Collider.h"
#include "RigidBody.h"
#include "SphereCollider.h"

#include "Entity.h"

class Scene {
private:
	uint32_t nextEntity = 0;

	std::unordered_map<uint32_t, Transform> transforms = {};
	std::unordered_map<uint32_t, MeshRenderer> mesh_renderers = {};
	std::unordered_map<uint32_t, SphereCollider> colliders = {};
	std::unordered_map<uint32_t, RigidBody> rigidbodies = {};
public:
	Scene() = default;
	Entity CreateEntity();

	// Physics
	float gravity = -9.82;


	// Environment
	float ambient = 0.2f;
	glm::vec3 light_direction = glm::normalize(glm::vec3(-1, -1.3, -0.84));
	glm::vec3 light_color = glm::vec3(1.0);



	// Specializations for each component type
	template<typename T>
	std::unordered_map<uint32_t, T>& GetComponentMap();

	template<typename T, typename... Args>
	T& AddComponent(const Entity& entity, Args&&... args) {
		auto& map = GetComponentMap<T>();
		return map[entity.id] = T(std::forward<Args>(args)...);
	}

	template<typename T>
	T& GetComponent(const Entity& entity) {
		auto& map = GetComponentMap<T>();
		auto it = map.find(entity.id);
		if (it != map.end()) {
			return it->second;
		}
		else {
			throw std::runtime_error("Component not found");
		}
	}

	template<typename T>
	void RemoveComponent(const Entity& entity) {
		auto& map = GetComponentMap<T>();
		auto it = map.find(entity.id);
		if (it != map.end()) {
			map.erase(it);
		}
		else {
			std::cout << "Component not found" << std::endl;
		}
	}

	void DrawMeshes(Camera& camera);
	void CheckCollisions(uint32_t id);
	void ResolveCollision(glm::vec3 collision_normal, RigidBody& rb1, RigidBody& rb2);
	void UpdatePhysics();

};

// Specialize the template for each component type
template<>
inline std::unordered_map<uint32_t, Transform>& Scene::GetComponentMap<Transform>() {
	return transforms;
}

template<>
inline std::unordered_map<uint32_t, MeshRenderer>& Scene::GetComponentMap<MeshRenderer>() {
	return mesh_renderers;
}

template<>
inline std::unordered_map<uint32_t, SphereCollider>& Scene::GetComponentMap<SphereCollider>() {
	return colliders;
}

template<>
inline std::unordered_map<uint32_t, RigidBody>& Scene::GetComponentMap<RigidBody>() {
	return rigidbodies;
}



#endif  