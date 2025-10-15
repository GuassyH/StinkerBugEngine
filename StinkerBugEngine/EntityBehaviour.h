#ifndef ENTITY_BEHAVIOUR_H
#define ENTITY_BEHAVIOUR_H

#include <memory>
#include "glm/glm.hpp"
#include "GLFW/glfw3.h"
#include "Component.h"

#include "Constants.h"
#include "DeltaTime.h"

#include "ComponentsList.h"

class EntityHelper;

class EntityBehaviour : public Component {
public:
	std::shared_ptr<EntityHelper> entityHelper = nullptr;

	EntityBehaviour() = default;
	virtual ~EntityBehaviour() = default;
	virtual void Init() override;
	virtual void Start() {}
	virtual void Awake() {}
	virtual void Update() {}
	virtual void PhysicsUpdate() {}
	virtual void OnCollisionEnter(Collider& other) {}
};





#endif