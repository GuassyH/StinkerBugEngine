#ifndef COLLISION_INFO_H
#define COLLISION_INFO_H

#include "glm/glm.hpp"

struct CollisionInfo {
    glm::vec3 position = glm::vec3(0.0f);
    glm::vec3 normal = glm::vec3(0.0f);
    bool did_collide = false;
    float penetration = 0.0f;
};

#endif