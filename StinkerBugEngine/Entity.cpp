#include "Entity.h"
#include "Scene.h"

Entity::Entity(uint32_t id, Scene* scene) : id(id), scene(scene) {}

/*
template<typename T, typename... Args>
T& Entity::AddComponent(Args&&... args) {
	return scene->AddComponent<T>(*this, std::forward<Args>(args)...);
}
*/
