#include "EntityBehaviour.h"
#include "Entity.h"
#include "SceneManager.h"

void EntityBehaviour::Init() {
    auto& scene = SceneManager::getInstance().GetActiveScene();

    if (parent_id >= scene.Scene_ECS.entities.size()) {
        std::cout << "Invalid parent_id: " << parent_id << std::endl;
        return;
    }

    entity = &scene.Scene_ECS.entities[parent_id];
    if (!entity) {
        std::cout << "Entity pointer is nullptr!" << std::endl;
        return;
    }

    // Check if entity has Transform
    if (!entity->HasComponent<Transform>()) {
        std::cout << "Entity " << std::to_string(parent_id) << " does not have a Transform component!" << std::endl;
        return;
    }

    transform = &entity->GetComponent<Transform>();
    std::cout << "Transform component for EntityBehaviour " << std::to_string(parent_id) << " obtained successfully." << std::endl;
}