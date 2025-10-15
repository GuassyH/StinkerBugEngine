#ifndef ENTITY_HELPER_H
#define ENTITY_HELPER_H

#include <cstdint>
#include "Entity.h"

#include "ECSystem.h"

class EntityHelper {
public:
	Entity id;

	EntityHelper() = default;
	EntityHelper(const Entity& id, ECSystem* ecs) : id(id), ecs(ecs) {}
	~EntityHelper() = default;

	// For normal components
	template<typename T, typename... Args>
	T& AddComponent(Args&&... args)
	{
		return ecs->AddComponent<T>(id, std::forward<Args>(args)...);
	}

	template<typename T, typename... Args>
	std::shared_ptr<T> AddComponentPtr(Args&&... args)
	{
		return ecs->AddComponentPtr<T>(id, std::forward<Args>(args)...);
	}


	template<typename T>
	T& GetComponent() {
		return ecs->GetComponent<T>(id);
	}

	template<typename T>
	std::shared_ptr<T> GetComponentPtr() {
		return ecs->GetComponentPtr<T>(id);
	}

	template<typename T>
	void RemoveComponent() {
		ecs->RemoveComponent<T>(id);
	}

	template<typename T>
	bool HasComponent() {
		return ecs->HasComponent<T>(id);
	}

	ECSystem* ecs;
};

#endif