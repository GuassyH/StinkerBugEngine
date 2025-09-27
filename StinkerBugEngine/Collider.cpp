#include "Collider.h"
#include "SceneManager.h"
#include "EntityHelper.h"
#include "ECSystem.h"
#include "Scene.h"

void Collider::Init() {
    auto& scene = SceneManager::getInstance().GetActiveScene();
    entityHelper = new EntityHelper(entity, &scene.Scene_ECS.WorldRegistry);

    // Check if entity has Transform
    if (!entityHelper->HasComponent<Transform>()) {
        std::cout << "Entity does not have a Transform component!" << std::endl;
        return;
    }
    transform = &entityHelper->GetComponent<Transform>();
}