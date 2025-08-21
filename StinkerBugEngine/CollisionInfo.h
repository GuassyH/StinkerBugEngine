#ifndef COLLISION_INFO_H
#define COLLISION_INFO_H

#include "glm/glm.hpp"

struct CollisionInfo {
    glm::vec3 collision_pos = glm::vec3(0.0f);
    glm::vec3 collision_normal = glm::vec3(0.0f);
    bool did_collide = false;
};

#endif