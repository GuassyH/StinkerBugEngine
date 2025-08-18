#ifndef ENTITY_BEHAVIOUR_H
#define ENTITY_BEHAVIOUR_H

#include <memory>
#include "glm/glm.hpp"
#include "GLFW/glfw3.h"

#include "Constants.h"
#include "DeltaTime.h"

#include "ComponentsList.h"

class Entity;

class EntityBehaviour {
public:
	uint32_t parent_id;
	Entity* entity;
	Transform* transform;

	EntityBehaviour() = default;
	virtual ~EntityBehaviour() = default;
	virtual void Init();
	virtual void Start() {}
	virtual void Awake() {}
	virtual void Update() {}
	virtual void PhysicsUpdate() {}
};





#endif