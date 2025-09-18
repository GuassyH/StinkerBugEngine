#ifndef ENTITY_COMPONENT_SYSTEM_H
#define ENTITY_COMPONENT_SYSTEM_H

#include <unordered_map>
#include <unordered_set>
#include <bitset>
#include <string>
#include <iostream>
#include <typeindex>
#include <typeinfo>

#include "EntityBehaviour.h"
#include "Collider.h"

#include "Entity.h"
#include "ComponentTypeID.h"

class ECSystem {
public:
	Entity nextEntity = 0;
	std::unordered_set<Entity> entities;
	std::unordered_map<Entity, std::string> entity_names;

	std::unordered_map<Entity, std::unique_ptr<Collider>> colliders;
	std::unordered_map<Entity, std::unique_ptr<EntityBehaviour>> entity_behaviours;

	// Save 32 bit unsigned ints to store info on if a component is held
	std::unordered_map<Entity, uint32_t> component_bits;
	std::unordered_map<std::type_index, std::unordered_map<Entity, std::shared_ptr<Component>>> components;

	void DestroyEntity(Entity id) {
		// remove all tracked things
		entities.erase(id);
		entity_names.erase(id);
		component_bits.erase(id);
		colliders.erase(id);
		entity_behaviours.erase(id);

		// erase from component maps
		for (auto& [typeIdx, map] : components) {
			map.erase(id);
		}
	}


	ECSystem() = default;
	~ECSystem() = default;



	void AddComponentBit(uint32_t id, Entity entity) {
		uint32_t& original_bits = component_bits[entity];
		original_bits |= id;
	}
	void RemoveComponentBit(uint32_t id, Entity entity) {
		auto it = component_bits.find(entity);
		if (it == component_bits.end()) return;
		uint32_t& original_bits = it->second;
		original_bits &= ~id;
	}

	bool HasComponentBit(uint32_t id, Entity entity) {
		auto it = component_bits.find(entity);
		if (it == component_bits.end()) return false;
		uint32_t& original_bits = it->second;
		return (original_bits & id) == id;
	}


	template<typename T>
	std::enable_if_t<!std::is_base_of_v<EntityBehaviour, T> && !std::is_base_of_v<Collider, T>, bool>
		HasComponent(const Entity id) {
		return HasComponentBit(ComponentBit<T>(), id);
	}

	template<typename T>
	std::enable_if_t<std::is_base_of_v<EntityBehaviour, T> || std::is_base_of_v<Collider, T>, bool>
		HasComponent(const Entity id) {
		if constexpr (std::is_base_of_v<Collider, T>) {
			auto it = colliders.find(id);
			return (it != colliders.end());
		}
		else {
			auto it = entity_behaviours.find(id);
			return (it != entity_behaviours.end());
		}
	}

	template<typename T>
	std::unordered_map<Entity, std::shared_ptr<Component>>& GetComponentMap() {
		return components[std::type_index(typeid(T))];
	}

	template<typename T>
	std::enable_if_t<std::is_base_of_v<EntityBehaviour, T> || std::is_base_of_v<Collider, T>, T&>
		GetComponent(Entity id) {
		// Pointer to the container we'll use
		if constexpr (std::is_base_of_v<Collider, T>) {
			auto it = colliders.find(id);
			if (it == colliders.end()) throw std::runtime_error("Collider not found for entity " + std::to_string(id));
			T* derived = dynamic_cast<T*>(it->second.get());
			if (!derived) throw std::runtime_error("Collider type mismatch for entity " + std::to_string(id));
			return *derived;
		}
		else { // EntityBehaviour
			auto it = entity_behaviours.find(id);
			if (it == entity_behaviours.end()) throw std::runtime_error("EntityBehaviour not found for entity " + std::to_string(id));
			T* derived = dynamic_cast<T*>(it->second.get());
			if (!derived) throw std::runtime_error("EntityBehaviour type mismatch for entity " + std::to_string(id));
			return *derived;
		}
	}

	template<class>
	static inline constexpr bool always_false = false;
	template<typename T>
	std::enable_if_t<!std::is_base_of_v<EntityBehaviour, T> && !std::is_base_of_v<Collider, T>, T&>
		GetComponent(const Entity id) {
		if (!HasComponent<T>(id)) {
			throw std::runtime_error("Component not found on entity " + std::to_string(id));
		}
		auto& map = GetComponentMap<T>();
		auto it = map.find(id);
		if (it == map.end()) {
			throw std::runtime_error("Internal error: component bit set but component missing for entity " + std::to_string(id));
		}
		return *std::static_pointer_cast<T>(it->second);
	}


	// In case its an entity behaviour collider (since they are polymorphic)
	template<typename T, typename... Args>
	std::enable_if_t<std::is_base_of_v<EntityBehaviour, T> || std::is_base_of_v<Collider, T>, T&>
		AddComponent(const Entity id, Args&&... args){
		// Pointer to the container we'll use
		if constexpr (std::is_base_of_v<Collider, T>) {
			colliders[id] = std::make_unique<T>(std::forward<Args>(args)...);
			colliders[id]->entity = id;
			colliders[id]->Init();
			return GetComponent<T>(id);
		}
		else if constexpr (std::is_base_of_v<EntityBehaviour, T>) {
			entity_behaviours[id] = std::make_unique<T>(std::forward<Args>(args)...);
			entity_behaviours[id]->entity = id;
			entity_behaviours[id]->Init();
			return GetComponent<T>(id);
		}
		else {
			static_assert(always_false<T>, "T must be derived from Collider or EntityBehaviour");
		}
	}

	// For normal components
	template<typename T, typename... Args>
	std::enable_if_t<!std::is_base_of_v<EntityBehaviour, T> && !std::is_base_of_v<Collider, T> && std::is_base_of_v<Component, T>, T&>
		AddComponent(const Entity id, Args&&... args)
	{
		auto& map = GetComponentMap<T>();
		if (HasComponent<T>(id)) { std::cout << "Entity: " << entity_names[id] << " already has component\n"; return *std::static_pointer_cast<T>(map.find(id)->second); }

		AddComponentBit(ComponentBit<T>(), id);
		std::cout << entity_names[id] << " - " << std::bitset<32>(component_bits[id]) << "\n";

		map[id] = std::make_shared<T>(std::forward<Args>(args)...);
		return GetComponent<T>(id);
	}



	template<typename T>
	void RemoveComponent(const Entity id) {
		if constexpr (std::is_base_of_v<Collider, T>) { colliders.erase(id); return; }
		else if constexpr (std::is_base_of_v<EntityBehaviour, T>) { entity_behaviours.erase(id); return; }

		if (HasComponent<T>(id)) {
			auto& map = GetComponentMap<T>();
			map.erase(id);

			RemoveComponentBit(ComponentBit<T>(), id);
			std::cout << entity_names[id] << " - " << std::bitset<32>(component_bits[id]) << "\n";
		}
		else {
			std::cout << "Component not found" << std::endl;
		}
	}

};




#endif