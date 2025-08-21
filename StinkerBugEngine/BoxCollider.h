#ifndef BOX_COLLIDER_COMPONENT_H
#define BOX_COLLIDER_COMPONENT_H

#include "Collider.h"
#include "ColliderFunctions.h"


class BoxCollider : public Collider {
public:

    CollisionInfo CollideWithBox(BoxCollider& other_box_collider) override;
    CollisionInfo CollideWithSphere(SphereCollider& other_sphere_collider) override;

    CollisionInfo CheckCollisions(Collider& other) override {
        if (!ColliderFunctions::AABB(*this, other)) { CollisionInfo info; info.did_collide = false; return info; }
        return other.CollideWithBox(*this);
    }
};


#endif