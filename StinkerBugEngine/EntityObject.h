#ifndef ENTITY_OBJECT_H
#define ENTITY_OBJECT_H

#include <cstdint>
#include "Entity.h"

#include "Transform.h"
#include "WeakPtrProxy.h"

class EntityObject {
public:
	EntityObject() = default;
	~EntityObject() = default;

	WeakPtrProxy<Transform> transform = nullptr;
};

#endif