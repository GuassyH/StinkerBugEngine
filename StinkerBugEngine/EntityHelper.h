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


	template<typename T>
	T& GetComponent() {
		return ecs->GetComponent<T>(id);
	}

	template<typename T>
	void RemoveComponent() {
		ecs->RemoveComponent<T>(id);
	}

	template<typename T>
	bool HasComponent() {
		return ecs->HasComponent<T>(id);
	}

private:
	ECSystem* ecs;
};

#endif