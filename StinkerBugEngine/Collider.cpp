#include "Collider.h"
#include "SceneManager.h"
#include "EntityObject.h"
#include "ECSystem.h"
#include "Scene.h"

void Collider::Init() {
    // Check if entity has Transform
    if (!transform) {
        throw std::runtime_error("EntityBehaviour for Entity " + std::to_string(entity) + " does not have a Transform component!");
    }
}