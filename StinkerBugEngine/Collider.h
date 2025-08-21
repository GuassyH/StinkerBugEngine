#ifndef COLLIDER_COMPONENT_H
#define COLLIDER_COMPONENT_H

#include <iostream>
#include <memory>

#include "glm/glm.hpp"
#include "Transform.h"
#include "CollisionInfo.h"

class Entity;

class SphereCollider;
class BoxCollider;

class Collider {
public:
    int parent_id;
    Transform* transform;
    Entity* entity;

    float radius = 0.5f;
    glm::vec3 offset = glm::vec3(0.5f);
    glm::vec3 size = glm::vec3(0.5f);

    Collider() = default;
    virtual ~Collider() = default; // Always virtual destructor for base classes
    virtual void Init();

	virtual CollisionInfo CheckCollisions(Collider& other) = 0;
    virtual CollisionInfo CollideWithBox(BoxCollider& other_box_collider) = 0;
    virtual CollisionInfo CollideWithSphere(SphereCollider& other_sphere_collider) = 0;
};


#endif