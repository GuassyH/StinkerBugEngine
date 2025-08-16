#ifndef ENTITY_INL
#define ENTITY_INL

#include "Scene.h"

template<typename T, typename... Args>
T& Entity::AddComponent(Args&&... args) {
	return scene->template AddComponent<T>(*this, std::forward<Args>(args)...);
}

#endif
