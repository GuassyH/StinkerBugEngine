#include "Collider.h"
#include "Entity.h"
#include "SceneManager.h"

void Collider::Init() {
    auto scene = SceneManager::getInstance().GetActiveScene();
    if (!scene) {
        std::cout << "Active scene is nullptr!" << std::endl;
        return;
    }

    if (parent_id >= scene->Scene_ECS.entities.size()) {
        std::cout << "Invalid parent_id: " << parent_id << std::endl;
        return;
    }

    entity = &scene->Scene_ECS.entities[parent_id];
    if (!entity) {
        std::cout << "Entity pointer is nullptr!" << std::endl;
        return;
    }

    // Check if entity has Transform
    if (!entity->HasComponent<Transform>()) {
        std::cout << "Entity does not have a Transform component!" << std::endl;
        return;
    }
    transform = &entity->GetComponent<Transform>();
    std::cout << "Transform component obtained successfully. " << std::to_string(parent_id) << std::endl;
}