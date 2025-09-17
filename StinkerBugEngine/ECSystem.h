#ifndef ENTITY_COMPONENT_SYSTEM_H
#define ENTITY_COMPONENT_SYSTEM_H

#include <unordered_map>
#include <unordered_set>
#include <bitset>
#include <string>
#include <iostream>

#include "ComponentsList.h"
#include "Entity.h"
#include "EntityBehaviour.h"
#include "TypeID.h"

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

	// Save 32 bit unsigned ints to store info on if a component is held
	std::unordered_map<Entity, uint32_t> component_bits;


// ALL THE IMPORTANT SHIZZ
	ECSystem() = default;



	void AddComponentBit(uint32_t id, Entity entity) {
		if (component_bits.find(entity) == component_bits.end()) { return; }

		uint32_t& original_bits = component_bits[entity];
		original_bits |= id;
	}
	void RemoveComponentBit(uint32_t id, Entity entity) {
		if (component_bits.find(entity) == component_bits.end()) { return; }

		uint32_t& original_bits = component_bits[entity];
		uint32_t mask_bits = ~id;
		original_bits &= id;
	}
	bool HasComponentBit(uint32_t id, Entity entity) {
		if (component_bits.find(entity) == component_bits.end()) { return false; }

		uint32_t& original_bits = component_bits[entity];
		uint32_t compare_bits = original_bits & id;
		return compare_bits == id;
	}

	// Specializations for each component type
	template<typename T>
	std::unordered_map<Entity, T>& GetComponentMap();


	template<typename T>
	std::enable_if_t<std::is_base_of_v<EntityBehaviour, T> || std::is_base_of_v<Collider, T>, T&>
		GetComponent(Entity id) {
		// Pointer to the container we'll use
		auto* containerPtr =
			[&]() -> auto* {
			if constexpr (std::is_base_of_v<Collider, T>) {
				return &colliders;
			}
			else if constexpr (std::is_base_of_v<EntityBehaviour, T>) {
				return &entity_behaviours;
			}
			else {
				static_assert(always_false<T>, "T must be derived from Collider or EntityBehaviour");
			}
			}();

		auto& container = *containerPtr;

		auto it = container.find(id);
		if (it == container.end()) {
			throw std::runtime_error("Component not found for entity " + std::to_string(id));
		}

		T* derived = static_cast<T*>(it->second.get());
		if (!derived) {
			throw std::runtime_error("Component type mismatch for entity " + std::to_string(id));
		}

		return *derived;
	}

	// Helper for static_assert in unreachable branch
	template<class>
	static inline constexpr bool always_false = false;

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
		if (HasComponentBit(ComponentBit<T>(), id)) { std::cout << "Entity: " << entity_names[id] << " already has component\n"; return map[id]; }
		
		AddComponentBit(ComponentBit<T>(), id);
		std::cout << entity_names[id] << " - " << std::bitset<32>(component_bits[id]) << "\n";

		map[id] = T(std::forward<Args>(args)...);
		map[id].entity = id;
		return map[id];
	}

	template<typename T>
	std::enable_if_t<!std::is_base_of_v<EntityBehaviour, T> && !std::is_base_of_v<Collider, T>, bool>
		HasComponent(const Entity id){
		return HasComponentBit(ComponentBit<T>(), id);
	}

	template<typename T>
	void RemoveComponent(const Entity id) {
		auto& map = GetComponentMap<T>();
		if (HasComponent<T>(id)) {
			auto& it = map.find(id);
			map.erase(it);

			RemoveComponentBit(ComponentBit<T>(), id);
			std::cout << entity_names[id] << " - " << std::bitset<32>(component_bits[id]) << "\n";
		}
		else {
			std::cout << "Component not found" << std::endl;
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