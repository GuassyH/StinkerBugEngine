#ifndef ENTITY_COMPONENT_SYSTEM_H
#define ENTITY_COMPONENT_SYSTEM_H

#include <unordered_map>

#include "ComponentsList.h"
#include "EntityBehaviour.h"

class ECSystem {
public:
	std::unordered_map<uint32_t, std::string> entity_names;
	std::unordered_map<uint32_t, Transform> transforms;
	std::unordered_map<uint32_t, MeshRenderer> mesh_renderers;
	std::unordered_map<uint32_t, SphereCollider> colliders;
	std::unordered_map<uint32_t, RigidBody> rigidbodies;
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
	}

	// For normal components
	template<typename T, typename... Args>
	std::enable_if_t<!std::is_base_of_v<EntityBehaviour, T>, T&>
		AddComponent(const uint32_t id, Args&&... args)
	{
		auto& map = GetComponentMap<T>();
		return map[id] = T(std::forward<Args>(args)...);
	}

	template<typename T>
	T& GetComponent(const uint32_t id) {
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
inline std::unordered_map<uint32_t, SphereCollider>& ECSystem::GetComponentMap<SphereCollider>() {
	return colliders;
}

template<>
inline std::unordered_map<uint32_t, RigidBody>& ECSystem::GetComponentMap<RigidBody>() {
	return rigidbodies;
}

template<>
inline std::unordered_map<uint32_t, std::unique_ptr<EntityBehaviour>>& ECSystem::GetComponentMap<std::unique_ptr<EntityBehaviour>>() {
	return entity_behaviours;
}



#endif