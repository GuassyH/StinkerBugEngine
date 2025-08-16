#ifndef ENTITY_ID_H
#define ENTITY_ID_H

#include <cstdint>

class Scene;

class Entity {
public:
	uint32_t id;

	Entity(uint32_t id, Scene* scene);
	//template<typename T, typename... Args>
	//T& AddComponent(Args&&... args);

private:
	Scene* scene;
};



#endif // !ENTITY_ID_H
