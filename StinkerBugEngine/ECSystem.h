#ifndef ENTITY_COMPONENT_SYSTEM_H
#define ENTITY_COMPONENT_SYSTEM_H

#include <unordered_map>
#include <unordered_set>
#include <bitset>
#include <string>
#include <iostream>
#include <typeindex>
#include <typeinfo>
#include <memory>

#include "EntityBehaviour.h"
#include "Collider.h"

#include "Component.h"
#include "Entity.h"
#include "ComponentTypeID.h"
#include "Renderer.h"
#include "MeshRenderer.h"
#include "Transform.h"

class ECSystem {
public:
	Entity nextEntity = 0;
	std::unordered_set<Entity> entities;
	std::unordered_map<Entity, std::string> entity_names;

	std::unordered_map<Entity, std::shared_ptr<Collider>> colliders;
	std::unordered_map<Entity, std::shared_ptr<EntityBehaviour>> entity_behaviours;

	std::unordered_map<Entity, uint32_t> component_bits;
	std::unordered_map<std::type_index, std::unordered_map<Entity, std::shared_ptr<Component>>> components;


	void DestroyEntity(Entity& id) {
		bool update_renderer = false;
		if (GetComponentMap<MeshRenderer>().find(id) != GetComponentMap<MeshRenderer>().end()) {
			if (components[std::type_index(typeid(GizmoComponent))].find(id) == components[std::type_index(typeid(GizmoComponent))].end()) {
				update_renderer = true;
			}
		}
	
		// remove all tracked things
		entities.erase(id);
		entity_names.erase(id);
		colliders.erase(id);
		entity_behaviours.erase(id);
		component_bits.erase(id);

		// erase from component maps
		for (auto& [typeIdx, map] : components) {
			map.erase(id);
		}

		if (update_renderer) {
			Renderer::getInstance().rebuildMeshLists(components);
		}
	}

	void DuplicateEntity(Entity& id) {

	}

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
	std::enable_if_t<std::is_base_of_v<Component, T>, bool> HasComponent(const Entity id) {
		if constexpr (std::is_base_of_v<Collider, T>) {
			auto it = colliders.find(id);
			return (it != colliders.end());
		}
		else if constexpr(std::is_base_of_v<EntityBehaviour, T>) {
			auto it = entity_behaviours.find(id);
			return (it != entity_behaviours.end());
		}
		else {
			return HasComponentBit(ComponentBit<T>(), id);
		}
		return false;
	}

	template<typename T>
	std::unordered_map<Entity, std::shared_ptr<Component>>& GetComponentMap() {
		return components[std::type_index(typeid(T))];
	}

	template<typename T>
	std::enable_if_t<std::is_base_of_v<Component, T>, T&> GetComponent(const Entity id) {
		if constexpr (std::is_base_of_v<Collider, T>) {
			auto it = colliders.find(id);
			if (it == colliders.end()) throw std::runtime_error("Collider not found for entity " + std::to_string(id));
			T* derived = dynamic_cast<T*>(it->second.get());
			if (!derived) throw std::runtime_error("Collider type mismatch for entity " + std::to_string(id));
			return *derived;
		}
		else if constexpr(std::is_base_of_v<EntityBehaviour, T>) { // EntityBehaviour
			auto it = entity_behaviours.find(id);
			if (it == entity_behaviours.end()) throw std::runtime_error("EntityBehaviour not found for entity " + std::to_string(id));
			T* derived = dynamic_cast<T*>(it->second.get());
			if (!derived) throw std::runtime_error("EntityBehaviour type mismatch for entity " + std::to_string(id));
			return *derived;
		}
		else {
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
	}

	template<typename T>
	std::enable_if_t<std::is_base_of_v<Component, T>, std::shared_ptr<T>> GetComponentPtr(const Entity id) // Return the std::shared_ptr
	{
		if constexpr (std::is_base_of_v<Collider, T>) {
			auto it = colliders.find(id);
			if (it == colliders.end()) throw std::runtime_error("Collider not found for entity " + std::to_string(id));
			return std::dynamic_pointer_cast<T>(it->second);
		}
		else if constexpr (std::is_base_of_v<EntityBehaviour, T>) { // EntityBehaviour
			auto it = entity_behaviours.find(id);
			if (it == entity_behaviours.end()) throw std::runtime_error("EntityBehaviour not found for entity " + std::to_string(id));
			return std::dynamic_pointer_cast<T>(it->second);
		}
		else {
			if (!HasComponent<T>(id)) {
				throw std::runtime_error("Component not found on entity " + std::to_string(id));
			}
			auto& map = GetComponentMap<T>();
			auto it = map.find(id);
			if (it == map.end()) {
				throw std::runtime_error("Internal error: component bit set but component missing for entity " + std::to_string(id));
			}
			return  std::dynamic_pointer_cast<T>(it->second);
		}
	}

	// For normal components
	template<typename T, typename... Args>
	std::enable_if_t<std::is_base_of_v<Component, T>, T&> AddComponent(const Entity id, Args&&... args)
	{
		// Pointer to the container we'll use
		if constexpr (std::is_base_of_v<Collider, T>) {
			colliders[id] = std::make_shared<T>(std::forward<Args>(args)...);
			colliders[id]->entity = id;
			colliders[id]->Init();
		}
		else if constexpr (std::is_base_of_v<EntityBehaviour, T>) {
			entity_behaviours[id] = std::make_shared<T>(std::forward<Args>(args)...);
			entity_behaviours[id]->entity = id;
			entity_behaviours[id]->Init();
		}
		else {
			auto& map = GetComponentMap<T>();
			if (HasComponent<T>(id)) { std::cout << "Entity: " << entity_names[id] << " already has component\n"; return *std::static_pointer_cast<T>(map.find(id)->second); }

			AddComponentBit(ComponentBit<T>(), id);

			map.insert(std::make_pair(id, std::make_shared<T>(std::forward<Args>(args)...)));

			if constexpr (std::is_base_of_v<MeshRenderer, T>) {
				Renderer::getInstance().rebuildMeshLists(components);
			}
		}

		GetComponent<T>(id).entity = id;
		if constexpr (std::is_base_of_v<Component, T>) {
			if (HasComponent<Transform>(id)) {
				std::shared_ptr<Component> comp_ptr = GetComponentPtr<T>(id);
				auto component_expanded = std::static_pointer_cast<Component>(comp_ptr);
				if (!component_expanded) throw std::runtime_error("Invalid component cast");
				component_expanded->transform = GetComponentPtr<Transform>(id);
			}
			else {
				std::cout << "No transform!" << std::endl;
			}
		}

		return GetComponent<T>(id);
	}

	// For normal components
	template<typename T, typename... Args>
	std::enable_if_t<std::is_base_of_v<Component, T>, std::shared_ptr<T>> AddComponentPtr(const Entity id, Args&&... args) // Returns shared_ptr
	{
		AddComponent<T>(id);

		return GetComponentPtr<T>(id);
	}


	template<typename T>
	std::enable_if_t<std::is_base_of_v<Component, T>, void> RemoveComponent(const Entity id) {
		if constexpr (std::is_base_of_v<Collider, T>) { colliders.erase(id); return; }
		else if constexpr (std::is_base_of_v<EntityBehaviour, T>) { entity_behaviours.erase(id); return; }

		if (HasComponent<T>(id)) {
			auto& map = GetComponentMap<T>();
			map.erase(id);

			RemoveComponentBit(ComponentBit<T>(), id);
		}
		else {
			std::cout << "Component not found" << std::endl;
		}
	}

	ECSystem() = default;
	~ECSystem() = default;
};




#endif