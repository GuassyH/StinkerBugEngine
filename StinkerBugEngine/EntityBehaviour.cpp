#include "EntityBehaviour.h"

#include "SceneManager.h"
#include "ECSystem.h"
#include "Scene.h"
#include "EntityObject.h"


void EntityBehaviour::Init() {
    auto& scene = SceneManager::getInstance().GetActiveScene();
	entityObject = std::make_shared<EntityObject>();
    entityObject->transform = transform;

    // Check if entity has Transform
    if (!entityObject->transform->HasComponent<Transform>()) {
        std::cout << "EntityBehaviour for Entity " << std::to_string(entity) << " does not have a Transform component!" << std::endl;
        return;
    }
}