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

#include "Component.h"
#include "Entity.h"
#include "ComponentTypeID.h"
#include "Renderer.h"

class MeshRenderer;
class Transform;
class EntityBehaviour;
class Collider;

class ECSystem {
public:
	Entity nextEntity = 0;
	std::unordered_set<Entity> entities;
	std::unordered_map<Entity, std::string> entity_names;

	std::unordered_map<Entity, std::shared_ptr<Collider>> colliders;
	std::unordered_map<Entity, std::unordered_map<std::type_index, std::shared_ptr<EntityBehaviour>>> entity_behaviours;
	//std::unordered_map<Entity, std::shared_ptr<EntityBehaviour>> entity_behaviours;

	std::unordered_map<Entity, uint32_t> component_bits;
	std::unordered_map<std::type_index, std::unordered_map<Entity, std::shared_ptr<Component>>> components;


	void DestroyEntity(Entity& id);
	void DuplicateEntity(Entity& id);

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
	std::unordered_map<Entity, std::shared_ptr<Component>>& 
		GetComponentMap() {
		return components[std::type_index(typeid(T))];
	}

	template<typename T>
	std::enable_if_t<std::is_base_of_v<Component, T>, bool> 
		HasComponent(const Entity id) {
		if constexpr (std::is_base_of_v<Collider, T>) {
			return colliders.contains(id);
		}
		else if constexpr(std::is_base_of_v<EntityBehaviour, T>) {
			auto eIt = entity_behaviours.find(id);
			if (eIt == entity_behaviours.end()) return false;
			return eIt->second.contains(std::type_index(typeid(T)));
		}
		else {
			return HasComponentBit(ComponentBit<T>(), id);
		}
		return false;
	}


	template<typename T>
	std::enable_if_t<std::is_base_of_v<Component, T>, T&> 
		GetComponent(const Entity id) {
		if constexpr (std::is_base_of_v<Collider, T>) {
			if (!colliders.contains(id)) 
				throw std::runtime_error("Collider not found for entity " + std::to_string(id));
			
			T* derived = dynamic_cast<T*>(colliders.find(id)->second.get());
			
			if (!derived) 
				throw std::runtime_error("Collider type mismatch for entity " + std::to_string(id));
			
			return *derived;
		}
		else if constexpr(std::is_base_of_v<EntityBehaviour, T>) { // EntityBehaviour
			auto entityIt = entity_behaviours.find(id);
			if (entityIt == entity_behaviours.end())
				throw std::runtime_error("EntityBehaviour map not found for entity " + std::to_string(id));

			auto& behaviourMap = entityIt->second;
			auto behaviourIt = behaviourMap.find(std::type_index(typeid(T)));
			if (behaviourIt == behaviourMap.end())
				throw std::runtime_error("EntityBehaviour not found for entity " + std::to_string(id));

			T* derived = dynamic_cast<T*>(behaviourIt->second.get());
			if (!derived)
				throw std::runtime_error("EntityBehaviour type mismatch for entity " + std::to_string(id));

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
	std::enable_if_t<std::is_base_of_v<Component, T>, std::shared_ptr<T>> 
		GetComponentPtr(const Entity id) // Return the std::shared_ptr
	{
		if constexpr (std::is_base_of_v<Collider, T>) {
			if (colliders.contains(id)) throw std::runtime_error("Collider not found for entity " + std::to_string(id));

			auto ptr = std::dynamic_pointer_cast<T>(colliders.find(id)->second);
			if (!ptr) 
				throw std::runtime_error("Component type mismatch for entity " + std::to_string(id));
			
			return ptr;
		}
		else if constexpr (std::is_base_of_v<EntityBehaviour, T>) { // EntityBehaviour
			auto entityIt = entity_behaviours.find(id);
			if (entityIt == entity_behaviours.end())
				throw std::runtime_error("EntityBehaviour map not found for entity " + std::to_string(id));

			auto& behaviourMap = entityIt->second;
			auto behaviourIt = behaviourMap.find(std::type_index(typeid(T)));
			if (behaviourIt == behaviourMap.end())
				throw std::runtime_error("EntityBehaviour not found for entity " + std::to_string(id));

			auto ptr = std::dynamic_pointer_cast<T>(behaviourIt->second);
			if (!ptr)
				throw std::runtime_error("EntityBehaviour type mismatch for entity " + std::to_string(id));

			return ptr;
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
			return std::static_pointer_cast<T>(it->second);
		}
	}

	// For normal components
	template<typename T, typename... Args>
	std::enable_if_t<std::is_base_of_v<Component, T>, T&> 
		AddComponent(const Entity id, Args&&... args)
	{
		// Pointer to the container we'll use
		if constexpr (std::is_base_of_v<Collider, T>) {
			colliders[id] = std::make_shared<T>(std::forward<Args>(args)...);
		}
		else if constexpr (std::is_base_of_v<EntityBehaviour, T>) {
			auto& behaviourMap = entity_behaviours[id];
			auto typeKey = std::type_index(typeid(T));

			// Prevent duplicates
			if (behaviourMap.contains(typeKey))
				throw std::runtime_error("EntityBehaviour of this type already exists for entity " + std::to_string(id));

			// Construct and store
			auto behaviour = std::make_shared<T>(std::forward<Args>(args)...);
			behaviourMap[typeKey] = behaviour;
		}
		else {
			auto& map = GetComponentMap<T>();
			if (HasComponent<T>(id)) { std::cout << "Entity: " << entity_names[id] << " already has component\n"; return GetComponent<T>(id); }

			AddComponentBit(ComponentBit<T>(), id);

			map.insert(std::make_pair(id, std::make_shared<T>(std::forward<Args>(args)...)));

			if constexpr (std::is_base_of_v<MeshRenderer, T>) { Renderer::getInstance().queue_rebuild = true; }
		}


		auto& comp = GetComponent<T>(id);
		comp.entity = id;
		comp.parent_ecs = this;

		auto comp_ptr = GetComponentPtr<T>(id);
		if (!comp_ptr) throw std::runtime_error("Invalid component-set-transform cast");
		if (HasComponent<Transform>(id)) {
			comp_ptr->transform = GetComponentPtr<Transform>(id);
		}
		else {
			comp_ptr->transform = nullptr;
			std::cout << "No transform!" << std::endl;
		}

		comp.Init();

		return comp;
	}

	// For normal components
	template<typename T, typename... Args>
	std::enable_if_t<std::is_base_of_v<Component, T>, std::shared_ptr<T>> 
		AddComponentPtr(const Entity id, Args&&... args) // Returns shared_ptr
	{
		AddComponent<T>(id, std::forward<Args>(args)...);
		return GetComponentPtr<T>(id);
	}



	template<typename T>
	std::enable_if_t<std::is_base_of_v<Component, T>, void> 
		RemoveComponent(const Entity id) {
		if constexpr (std::is_base_of_v<Collider, T>) { colliders.erase(id); return; }
		else if constexpr (std::is_base_of_v<EntityBehaviour, T>) {
			auto entityIt = entity_behaviours.find(id);
			if (entityIt == entity_behaviours.end()) return;

			entityIt->second.erase(std::type_index(typeid(T)));
			if (entityIt->second.empty())
				entity_behaviours.erase(entityIt);
			return;
		}

		if (HasComponent<T>(id)) {
			auto& map = GetComponentMap<T>();
			map.erase(id);

			RemoveComponentBit(ComponentBit<T>(), id);

			if constexpr (std::is_base_of_v<MeshRenderer, T>) { Renderer::getInstance().queue_rebuild = true; }
		}
		else {
			std::cout << "Component not found - Nothing to remove" << std::endl;
		}
	}

	ECSystem() = default;
	~ECSystem() = default;
};




#endif