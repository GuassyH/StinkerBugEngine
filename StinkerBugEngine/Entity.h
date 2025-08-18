#ifndef ENTITY_H
#define ENTITY_H

#include <cstdint>
#include <utility>

#include "ECSystem.h"
#include "Scene.h"


class Entity {
public:
	uint32_t id;

	Entity(uint32_t id, Scene* scene) : id(id), scene(scene) {}

	// For EntityBehaviour
	template<typename T, typename... Args>
	std::enable_if_t<std::is_base_of_v<EntityBehaviour, T>, T>
		AddComponent(Args&&... args)
	{
		scene->Scene_ECS.AddComponent<T>(id, std::forward<Args>(args)...);
		return static_cast<T&>(*scene->Scene_ECS.entity_behaviours[id]);
	}

	// For normal components
	template<typename T, typename... Args>
	std::enable_if_t<!std::is_base_of_v<EntityBehaviour, T>, T&>
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

private:
	Scene* scene;
};


#endif
