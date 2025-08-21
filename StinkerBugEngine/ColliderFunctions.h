#ifndef COLLIDER_FUNCTIONS_H
#define COLLIDER_FUNCTIONS_H

#include "CollisionInfo.h"

class Collider;
class SphereCollider;
class BoxCollider;

namespace ColliderFunctions {
	bool AABB(Collider& first_collider, Collider& second_collider);

	CollisionInfo SphereVsSphere(SphereCollider& this_sphere, SphereCollider& other_sphere);
	CollisionInfo SphereVsBox(SphereCollider& sphere, BoxCollider& box);
	CollisionInfo BoxVsBox(BoxCollider& this_box, BoxCollider& other_box);
};


#endif