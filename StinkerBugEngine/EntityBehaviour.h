#ifndef ENTITY_BEHAVIOUR_H
#define ENTITY_BEHAVIOUR_H

#include <memory>
#include "glm/glm.hpp"
#include "GLFW/glfw3.h"
#include "Component.h"

#include "Constants.h"
#include "DeltaTime.h"
#include "Display.h"

#include "ComponentsList.h"
#include "WeakPtrProxy.h"

#include "Model.h"
#include "Material.h"

#include "SceneManager.h"
#include "Scene.h"

#include "EntityObject.h"
#include "ECSystem.h"

class EntityBehaviour : public Component {
public:

	EntityBehaviour() = default;
	virtual ~EntityBehaviour() = default;
	virtual void Init() override;
	virtual void Start() {}
	virtual void Awake() {}
	virtual void Update() {}
	virtual void PhysicsUpdate() {}
	virtual void OnCollisionEnter(Collider& other) {}

	EntityObject& CreateEntity() {
		EntityObject newObj = SceneManager::getInstance().GetActiveScene().CreateEntity(); 
		return newObj;
	}
	EntityObject& CreateEntity(std::string name) { 
		EntityObject newObj = SceneManager::getInstance().GetActiveScene().CreateEntity(name); 
		return newObj;
	}
};





#endif