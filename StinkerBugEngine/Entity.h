#ifndef ENTITY_H
#define ENTITY_H

#include <cstdint>
#include <utility>

class Scene;

class Entity {
public:
	uint32_t id;

	Entity(uint32_t id, Scene* scene);

private:
	Scene* scene;
};


#endif
