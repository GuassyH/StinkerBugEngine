#ifndef BOX_COLLIDER_COMPONENT_H
#define BOX_COLLIDER_COMPONENT_H

#include "Collider.h"
#include "ColliderFunctions.h"
#include "Vertex.h"

#include <iostream>
#include <vector>

class BoxCollider : public Collider {
public:
    std::vector<Vertex> vertices;
    std::vector<glm::vec3> vert_positions;

    void CalculateCorners();

    CollisionInfo CollideWithBox(BoxCollider& other_box_collider) override;
    CollisionInfo CollideWithSphere(SphereCollider& other_sphere_collider) override;

    CollisionInfo CheckCollisions(Collider& other) override {
        if (!ColliderFunctions::AABB(*this, other)) { CollisionInfo info; info.did_collide = false; return info; }
        return other.CollideWithBox(*this);
    }
private:
    bool init = true;
    glm::vec3 last_rotation = glm::vec3(0);
    glm::vec3 last_position = glm::vec3(0);
};


#endif