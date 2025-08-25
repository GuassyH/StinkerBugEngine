#ifndef ENTITY_H
#define ENTITY_H

#include <cstdint>
#include <utility>

#include "ECSystem.h"
#include "Scene.h"


class Entity {
public:
	uint32_t id;

	Entity() = default;
	Entity(uint32_t id, Scene* scene) : id(id), scene(scene) {}


	// For components that derive from other classes (Collider, EntityBehaviour)
	template<typename T, typename... Args>
	std::enable_if_t<std::is_base_of_v<Collider, T> || std::is_base_of_v<EntityBehaviour, T>, T&>
		AddComponent(Args&&... args)
	{
		return scene->Scene_ECS.AddComponent<T>(id, std::forward<Args>(args)...);
	}

	// For normal components
	template<typename T, typename... Args>
	std::enable_if_t<!std::is_base_of_v<EntityBehaviour, T> && !std::is_base_of_v<Collider, T>, T&>
		AddComponent(Args&&... args)
	{
		return scene->Scene_ECS.AddComponent<T>(id, std::forward<Args>(args)...);
	}


	template<typename T>
	T& GetComponent() {
		return scene->Scene_ECS.GetComponent<T>(id);
	}

	template<typename T>
	void RemoveComponent() {
		scene->Scene_ECS.RemoveComponent<T>(id);
	}

	template<typename T>
	bool HasComponent() {
		return scene->Scene_ECS.HasComponent<T>(id);
	}

private:
	Scene* scene;
};


#endif
