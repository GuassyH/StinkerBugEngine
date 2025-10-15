#ifndef ENTITY_OBJECT_H
#define ENTITY_OBJECT_H

#include <cstdint>
#include "Entity.h"

#include "Transform.h"

class EntityObject {
public:
	EntityObject() = default;
	~EntityObject() = default;

	std::shared_ptr<Transform> transform;
};

#endif