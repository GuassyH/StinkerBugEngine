#include "BoxCollider.h"



CollisionInfo BoxCollider::CollideWithBox(BoxCollider& other_box_collider) {
	return ColliderFunctions::BoxVsBox(*this, other_box_collider);
}

CollisionInfo BoxCollider::CollideWithSphere(SphereCollider& other_sphere_collider) {
	return ColliderFunctions::SphereVsBox(other_sphere_collider, *this);
}