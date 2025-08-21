#ifndef ENTITY_COMPONENT_SYSTEM_H
#define ENTITY_COMPONENT_SYSTEM_H

#include <unordered_map>

#include "ComponentsList.h"
#include "EntityBehaviour.h"

class ECSystem {
public:
	uint32_t nextEntity = 0;
	std::unordered_map<uint32_t, Entity> entities;
	std::unordered_map<uint32_t, std::string> entity_names;
	std::unordered_map<uint32_t, Transform> transforms;
	std::unordered_map<uint32_t, MeshRenderer> mesh_renderers;
	std::unordered_map<uint32_t, RigidBody> rigidbodies;
	std::unordered_map<uint32_t, Camera> cameras;
	std::unordered_map<uint32_t, Light> lights;

	std::unordered_map<uint32_t, std::unique_ptr<Collider>> colliders;
	std::unordered_map<uint32_t, std::unique_ptr<EntityBehaviour>> entity_behaviours;

// ALL THE IMPORTANT SHIZZ
	ECSystem() = default;

	// Specializations for each component type
	template<typename T>
	std::unordered_map<uint32_t, T>& GetComponentMap();

	// For EntityBehaviour
	template<typename T, typename... Args>
	std::enable_if_t<std::is_base_of_v<EntityBehaviour, T>>
		AddComponent(const uint32_t id, Args&&... args)
	{
		entity_behaviours[id] = std::make_unique<T>(std::forward<Args>(args)...);
		entity_behaviours[id]->parent_id = id;
		entity_behaviours[id]->Init();
	}

	// For EntityBehaviour
	template<typename T, typename... Args>
	std::enable_if_t<std::is_base_of_v<Collider, T>>
		AddComponent(const uint32_t id, Args&&... args)
	{
		colliders[id] = std::make_unique<T>(std::forward<Args>(args)...);
		colliders[id]->parent_id = id;
		colliders[id]->Init();
	}


	// For normal components
	template<typename T, typename... Args>
	std::enable_if_t<!std::is_base_of_v<EntityBehaviour, T> && !std::is_base_of_v<Collider, T>, T&>
		AddComponent(const uint32_t id, Args&&... args)
	{
		auto& map = GetComponentMap<T>();
		return map[id] = T(std::forward<Args>(args)...);
	}

	template<typename T>
	std::enable_if_t<!std::is_base_of_v<EntityBehaviour, T> && !std::is_base_of_v<Collider, T>, T&>
		GetComponent(const uint32_t id) {
		auto& map = GetComponentMap<T>();
		auto it = map.find(id);
		if (it != map.end()) {
			return it->second;
		}
		else {
			throw std::runtime_error("Component not found");
		}
	}

	template<typename T>
	std::enable_if_t<std::is_base_of_v<EntityBehaviour, T> || std::is_base_of_v<Collider, T>, T&>
		GetComponent(uint32_t id) {
		auto it = colliders.find(id); // or entity_behaviours if that's what you're accessing
		if (it == colliders.end()) {
			throw std::runtime_error("Collider not found for entity " + std::to_string(id));
		}

		T* derived = dynamic_cast<T*>(it->second.get());
		if (!derived) {
			throw std::runtime_error("Collider type mismatch for entity " + std::to_string(id));
		}

		return *derived;
	}


	template<typename T>
	void RemoveComponent(const uint32_t id) {
		auto& map = GetComponentMap<T>();
		auto it = map.find(id);
		if (it != map.end()) {
			map.erase(it);
		}
		else {
			std::cout << "Component not found" << std::endl;
		}
	}

	template<typename T>
	bool HasComponent(const uint32_t id) {
		auto& map = GetComponentMap<T>();
		auto it = map.find(id);
		if (it != map.end()) {
			return true;
		}
		else {
			return false;
		}
	}
};


// Specialize the template for each component type
template<>
inline std::unordered_map<uint32_t, Transform>& ECSystem::GetComponentMap<Transform>() {
	return transforms;
}

template<>
inline std::unordered_map<uint32_t, MeshRenderer>& ECSystem::GetComponentMap<MeshRenderer>() {
	return mesh_renderers;
}

template<>
inline std::unordered_map<uint32_t, RigidBody>& ECSystem::GetComponentMap<RigidBody>() {
	return rigidbodies;
}

template<>
inline std::unordered_map<uint32_t, Camera>& ECSystem::GetComponentMap<Camera>() {
	return cameras;
}

template<>
inline std::unordered_map<uint32_t, Light>& ECSystem::GetComponentMap<Light>() {
	return lights;
}

template<>
inline std::unordered_map<uint32_t, std::unique_ptr<Collider>>& ECSystem::GetComponentMap<std::unique_ptr<Collider>>() {
	return colliders;
}

template<>
inline std::unordered_map<uint32_t, std::unique_ptr<EntityBehaviour>>& ECSystem::GetComponentMap<std::unique_ptr<EntityBehaviour>>() {
	return entity_behaviours;
}



#endif