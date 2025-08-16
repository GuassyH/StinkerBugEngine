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

#include "Entity.h"

class Scene {
public:
	Scene() = default;


	glm::vec3 light_direction = glm::normalize(glm::vec3(-1, -1.3, -0.84));
	glm::vec3 light_color = glm::vec3(1.0);
	float ambient = 0.2f;

	Entity CreateEntity();

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

	void DrawMeshes(Camera& camera);
	void CheckCollisions();
// private:
	uint32_t nextEntity = 0;

	std::unordered_map<uint32_t, Transform> transforms = {};
	std::unordered_map<uint32_t, MeshRenderer> meshRenderers = {};
	std::unordered_map<uint32_t, std::unique_ptr<Collider>> colliders = {};
	std::unordered_map<uint32_t, RigidBody> rigidBodies = {};
};

// Specialize the template for each component type
template<>
inline std::unordered_map<uint32_t, Transform>& Scene::GetComponentMap<Transform>() {
	return transforms;
}

template<>
inline std::unordered_map<uint32_t, MeshRenderer>& Scene::GetComponentMap<MeshRenderer>() {
	return meshRenderers;
}

template<>
inline std::unordered_map<uint32_t, std::unique_ptr<Collider>>& Scene::GetComponentMap<std::unique_ptr<Collider>>() {
	return colliders;
}



#endif  