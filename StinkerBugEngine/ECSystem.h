#ifndef ENTITY_COMPONENT_SYSTEM_H
#define ENTITY_COMPONENT_SYSTEM_H

#include <unordered_map>
#include <unordered_set>

#include "ComponentsList.h"
#include "Entity.h"
#include "EntityBehaviour.h"

class ECSystem {
public:
	Entity nextEntity = 0;
	std::unordered_set<Entity> entities;
	std::unordered_map<Entity, std::string> entity_names;
	std::unordered_map<Entity, Transform> transforms;
	std::unordered_map<Entity, MeshRenderer> mesh_renderers;
	std::unordered_map<Entity, RigidBody> rigidbodies;
	std::unordered_map<Entity, Camera> cameras;
	std::unordered_map<Entity, Light> lights;

	std::unordered_map<Entity, std::unique_ptr<Collider>> colliders;
	std::unordered_map<Entity, std::unique_ptr<EntityBehaviour>> entity_behaviours;

// ALL THE IMPORTANT SHIZZ
	ECSystem() = default;

	// Specializations for each component type
	template<typename T>
	std::unordered_map<Entity, T>& GetComponentMap();


	template<typename T>
	std::enable_if_t<std::is_base_of_v<Collider, T>, T&>
		GetComponent(Entity id) {
		auto it = colliders.find(id); // or entity_behaviours if that's what you're accessing
		if (it == colliders.end()) {
			throw std::runtime_error("Collider / Behaviour not found for entity " + std::to_string(id));
		}

		T* derived = static_cast<T*>(it->second.get());
		if (!derived) {
			throw std::runtime_error("Collider / Behaviour type mismatch for entity " + std::to_string(id));
		}

		return *derived;
	}

	template<typename T>
	std::enable_if_t<std::is_base_of_v<EntityBehaviour, T>, T&>
		GetComponent(Entity id) {
		auto it = entity_behaviours.find(id); // or entity_behaviours if that's what you're accessing
		if (it == entity_behaviours.end()) {
			throw std::runtime_error("Collider / Behaviour not found for entity " + std::to_string(id));
		}

		T* derived = static_cast<T*>(it->second.get());
		if (!derived) {
			throw std::runtime_error("Collider / Behaviour type mismatch for entity " + std::to_string(id));
		}

		return *derived;
	}

	// For EntityBehaviour
	template<typename T, typename... Args>
	std::enable_if_t<std::is_base_of_v<EntityBehaviour, T>, T&>
		AddComponent(const Entity id, Args&&... args)
	{
		entity_behaviours[id] = std::make_unique<T>(std::forward<Args>(args)...);
		entity_behaviours[id]->entity = id;
		entity_behaviours[id]->Init();
		return GetComponent<T>(id);
	}

	// For EntityBehaviour
	template<typename T, typename... Args>
	std::enable_if_t<std::is_base_of_v<Collider, T>, T&>
		AddComponent(const Entity id, Args&&... args)
	{
		colliders[id] = std::make_unique<T>(std::forward<Args>(args)...);
		colliders[id]->entity = id;
		colliders[id]->Init();
		return GetComponent<T>(id);
	}


	// For normal components
	template<typename T, typename... Args>
	std::enable_if_t<!std::is_base_of_v<EntityBehaviour, T> && !std::is_base_of_v<Collider, T>, T&>
		AddComponent(const Entity id, Args&&... args)
	{
		auto& map = GetComponentMap<T>();
		map[id] = T(std::forward<Args>(args)...);
		map[id].entity = id;
		return map[id];
	}

	template<typename T>
	std::enable_if_t<!std::is_base_of_v<EntityBehaviour, T> && !std::is_base_of_v<Collider, T>, T&>
		GetComponent(const Entity id) {
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
	void RemoveComponent(const Entity id) {
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
	bool HasComponent(const Entity id) {
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
inline std::unordered_map<Entity, MeshRenderer>& ECSystem::GetComponentMap<MeshRenderer>() {
	return mesh_renderers;
}

template<>
inline std::unordered_map<Entity, RigidBody>& ECSystem::GetComponentMap<RigidBody>() {
	return rigidbodies;
}

template<>
inline std::unordered_map<Entity, Camera>& ECSystem::GetComponentMap<Camera>() {
	return cameras;
}

template<>
inline std::unordered_map<Entity, Light>& ECSystem::GetComponentMap<Light>() {
	return lights;
}

template<>
inline std::unordered_map<Entity, std::unique_ptr<Collider>>& ECSystem::GetComponentMap<std::unique_ptr<Collider>>() {
	return colliders;
}

template<>
inline std::unordered_map<Entity, std::unique_ptr<EntityBehaviour>>& ECSystem::GetComponentMap<std::unique_ptr<EntityBehaviour>>() {
	return entity_behaviours;
}



#endif