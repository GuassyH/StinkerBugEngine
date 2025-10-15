#include "Collider.h"
#include "SceneManager.h"
#include "EntityHelper.h"
#include "ECSystem.h"
#include "Scene.h"

void Collider::Init() {
    auto& scene = SceneManager::getInstance().GetActiveScene();
    entityHelper = std::make_shared<EntityHelper>(entity, &scene.Scene_ECS);

    // Check if entity has Transform
    if (!entityHelper->HasComponent<Transform>()) {
        std::cout << "Collider for Entity " << std::to_string(entity) << " does not have a Transform component!" << std::endl;
        return;
    }
}