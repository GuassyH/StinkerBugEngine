#include "EntityBehaviour.h"

#include "SceneManager.h"
#include "ECSystem.h"
#include "Scene.h"
#include "EntityHelper.h"


void EntityBehaviour::Init() {
    auto& scene = SceneManager::getInstance().GetActiveScene();
	entityHelper = new EntityHelper(entity, &scene.Scene_ECS.WorldRegistry);

    // Check if entity has Transform
    if (!entityHelper->HasComponent<Transform>()) {
        std::cout << "Entity " << std::to_string(entity) << " does not have a Transform component!" << std::endl;
        return;
    }

    transform = &entityHelper->GetComponent<Transform>();
}