#ifndef ENTITY_HELPER_H
#define ENTITY_HELPER_H

#include <cstdint>
#include "Entity.h"

#include "ECSystem.h"

class EntityHelper {
public:
	Entity id;

	EntityHelper() = default;
	EntityHelper(const Entity& id, ECS_Registry* reg) : id(id), registry(reg) {}
	~EntityHelper() = default;

	// For normal components
	template<typename T, typename... Args>
	T& AddComponent(Args&&... args)
	{
		return registry->AddComponent<T>(id, std::forward<Args>(args)...);
	}


	template<typename T>
	T& GetComponent() {
		return registry->GetComponent<T>(id);
	}

	template<typename T>
	void RemoveComponent() {
		registry->RemoveComponent<T>(id);
	}

	template<typename T>
	bool HasComponent() {
		return registry->HasComponent<T>(id);
	}

	ECS_Registry* registry;
};

#endif