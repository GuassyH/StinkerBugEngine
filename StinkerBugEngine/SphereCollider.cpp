#include "SphereCollider.h"



CollisionInfo SphereCollider::CollideWithBox(BoxCollider& other_box_collider) {
	return ColliderFunctions::SphereVsBox(*this, other_box_collider);
}

CollisionInfo SphereCollider::CollideWithSphere(SphereCollider& other_sphere_collider) {
	return ColliderFunctions::SphereVsSphere(*this, other_sphere_collider);
}