#include "EntityBehaviour.h"

#include "SceneManager.h"
#include "ECSystem.h"
#include "Scene.h"
#include "EntityObject.h"


void EntityBehaviour::Init() {
    // Check if entity has Transform
    if (!transform) {
        throw std::runtime_error("EntityBehaviour for Entity " + std::to_string(entity) + " does not have a Transform component!");
    }
}